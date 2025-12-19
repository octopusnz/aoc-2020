#!/usr/bin/env python3
"""Lint Makefiles for GNU make TAB-sensitive recipe lines.

GNU make requires each *recipe* line to start with a TAB character (unless
.RECIPEPREFIX is set). One common foot-gun is accidentally indenting a recipe
line with spaces, which triggers:

  *** missing separator.  Stop.

This linter scans all Makefiles in the repo and reports any *standalone*
space-indented recipe lines.

Important: it intentionally ignores physical lines that are just a continuation
of a previous logical line ending in a backslash ("\\"). Those continuation
lines do not require a TAB.
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Issue:
    path: str
    line: int
    target: str
    text: str


# Target header heuristic:
# - must start at column 0
# - must contain ':'
# - must not be a variable assignment like 'X:=' or 'X ='
TARGET_RE = re.compile(r"^[^\s#][^:=#]*:(?![=])")


def _ends_with_unescaped_backslash(raw: bytes) -> bool:
    stripped = raw.rstrip(b" \t")
    if not stripped.endswith(b"\\"):
        return False

    # Count trailing backslashes; odd => final backslash is unescaped.
    count = 0
    i = len(stripped) - 1
    while i >= 0 and stripped[i] == 0x5C:  # '\\'
        count += 1
        i -= 1
    return (count % 2) == 1


def _is_top_level_directive(line: str) -> bool:
    # Minimal set; good enough for this repo.
    return line.startswith(("ifeq", "ifneq", "ifdef", "ifndef", "else", "endif", "include"))


def lint_makefile(path: Path) -> list[Issue]:
    raw_lines = path.read_bytes().splitlines()

    issues: list[Issue] = []

    in_recipe = False
    current_target = ""
    prev_continues = False

    for idx, raw in enumerate(raw_lines, start=1):
        line = raw.decode("utf-8", errors="replace")

        # If this physical line is a continuation of the previous logical line,
        # it cannot be a standalone recipe line that needs a TAB.
        if prev_continues:
            prev_continues = _ends_with_unescaped_backslash(raw)
            continue

        if in_recipe:
            if line.strip() == "":
                in_recipe = False
                current_target = ""
                prev_continues = _ends_with_unescaped_backslash(raw)
                continue

            if line.startswith("#"):
                prev_continues = _ends_with_unescaped_backslash(raw)
                continue

            # If we hit a top-level directive or a new stanza at col 0, the recipe ends.
            if _is_top_level_directive(line) and not line.startswith("\t"):
                in_recipe = False
                current_target = ""
            elif not line.startswith(("\t", " ")):
                in_recipe = False
                current_target = ""

        if (not in_recipe) and TARGET_RE.match(line):
            in_recipe = True
            current_target = line.strip()
            prev_continues = _ends_with_unescaped_backslash(raw)
            continue

        if in_recipe and line.startswith(" "):
            issues.append(
                Issue(
                    path=str(path),
                    line=idx,
                    target=current_target,
                    text=line.rstrip("\n")[:200],
                )
            )

        prev_continues = _ends_with_unescaped_backslash(raw)

    return issues


def main() -> int:
    repo_root = Path(__file__).resolve().parents[1]
    makefiles = sorted(p for p in repo_root.rglob("Makefile") if p.is_file())

    all_issues: list[Issue] = []
    for mf in makefiles:
        all_issues.extend(lint_makefile(mf.relative_to(repo_root)))

    if not all_issues:
        print(f"OK: scanned {len(makefiles)} Makefiles; no space-indented recipe lines")
        return 0

    print(f"FAIL: found {len(all_issues)} space-indented recipe lines")
    for issue in all_issues:
        print(f"- {issue.path}:{issue.line} target={issue.target!r} line={issue.text!r}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
