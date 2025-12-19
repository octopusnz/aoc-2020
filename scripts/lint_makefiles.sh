#!/usr/bin/env bash
set -euo pipefail

# Lint Makefiles for GNU make TAB-sensitive recipe lines.
#
# GNU make requires each *recipe* line to start with a TAB character (unless
# .RECIPEPREFIX is set). A common foot-gun is indenting a recipe line with
# spaces, which triggers:
#   *** missing separator.  Stop.
#
# This linter scans all Makefiles in the repo and reports any *standalone*
# space-indented recipe lines.
#
# Important: it intentionally ignores physical lines that are just a
# continuation of a previous logical line ending in a backslash ("\\"). Those
# continuation lines do not require a TAB.

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

tmp_awk="$(mktemp -t lint_makefiles.XXXXXX.awk)"
trap 'rm -f "$tmp_awk"' EXIT

# Print issues as:
# - path:line target='...' line='...'
cat >"$tmp_awk" <<'AWK'
function rtrim(s) { sub(/[ \t]+$/, "", s); return s }

function ends_with_unescaped_backslash(s,    t, i, c, n) {
  t = rtrim(s)
  if (t !~ /\\$/) return 0
  n = 0
  for (i = length(t); i >= 1; i--) {
    c = substr(t, i, 1)
    if (c == "\\") n++
    else break
  }
  return (n % 2) == 1
}

function is_top_level_directive(line) {
  return (line ~ /^(ifeq|ifneq|ifdef|ifndef|else|endif|include)\b/)
}

function is_target_header(line,    colon_pos, after) {
  # Must start at column 0 (not space/tab/#) and contain a ":"
  if (line ~ /^[ \t#]/) return 0
  colon_pos = index(line, ":")
  if (colon_pos == 0) return 0
  # Exclude variable assignments like VAR:= ... (colon immediately followed by =)
  after = substr(line, colon_pos + 1, 1)
  if (after == "=") return 0
  return 1
}

BEGIN { in_recipe=0; prev_cont=0; current_target="" }
{
  line = $0

  # If this physical line is a continuation of the previous logical line,
  # it cannot be a standalone recipe line that needs a TAB.
  if (prev_cont) {
    prev_cont = ends_with_unescaped_backslash(line)
    next
  }

  if (in_recipe) {
    if (line ~ /^$/) {
      in_recipe=0; current_target=""; prev_cont = ends_with_unescaped_backslash(line); next
    }
    if (line ~ /^#/) {
      prev_cont = ends_with_unescaped_backslash(line); next
    }

    # If we hit a top-level directive or a new stanza at col 0, the recipe ends.
    if (is_top_level_directive(line) && line !~ /^\t/) {
      in_recipe=0; current_target=""
    } else if (line !~ /^[ \t]/) {
      in_recipe=0; current_target=""
    }
  }

  if (!in_recipe && is_target_header(line)) {
    in_recipe=1; current_target=line
    prev_cont = ends_with_unescaped_backslash(line)
    next
  }

  if (in_recipe && line ~ /^ /) {
    # Space-indented recipe line -> GNU make error
    printf("- %s:%d target=%c%s%c line=%c%s%c\n", FILENAME, FNR, 39, current_target, 39, 39, substr(line,1,200), 39)
    issues++
  }

  prev_cont = ends_with_unescaped_backslash(line)
}
END { exit (issues ? 2 : 0) }
AWK

makefile_count="$(find . -name Makefile -type f | wc -l | tr -d '[:space:]')"

if [[ "${makefile_count}" == "0" ]]; then
  echo "OK: scanned 0 Makefiles; none found" >&2
  exit 0
fi

# Run the awk program once over all files so we can get a single exit code.
# Use -print0/-0 so paths with spaces are handled safely.
set +e
output="$(find . -name Makefile -type f -print0 | LC_ALL=C sort -z | xargs -0 awk -f "$tmp_awk" 2>&1)"
status=$?
set -e

if [[ $status -eq 0 ]]; then
  echo "OK: scanned ${makefile_count} Makefiles; no space-indented recipe lines"
  exit 0
fi

# awk exits with 2 if issues were found.
if [[ $status -eq 2 ]]; then
  # Count issues from output lines.
  issue_count=$(printf "%s\n" "$output" | grep -c '^-' || true)
  echo "FAIL: found $issue_count space-indented recipe lines" >&2
  printf "%s\n" "$output" >&2
  exit 1
fi

echo "FAIL: linter error (awk exit $status)" >&2
printf "%s\n" "$output" >&2
exit 1
