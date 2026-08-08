#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

status=0

report_error() {
  printf 'ERROR: %s\n' "$*" >&2
  status=1
}

while IFS= read -r -d '' path; do
  if [[ "$path" =~ (^|/)(__pycache__|\.pytest_cache|\.mypy_cache|\.ruff_cache)(/|$) ]] ||
    [[ "$path" =~ (\.py[co]|\.bak|\.orig|\.rej|\.save|\.sw[op]|~|\.DS_Store)$ ]]; then
    report_error "tracked cache or backup file: $path"
  fi

  if [[ -f "$path" ]] && file -b -- "$path" | grep -q '^ELF '; then
    report_error "tracked ELF binary: $path"
  fi
done < <(git ls-files -z)

while IFS= read -r -d '' source; do
  example_dir="${source%/*}"
  if [[ ! -f "$example_dir/Makefile" ]]; then
    report_error "example source has no Makefile: $source"
  fi
done < <(find examples -mindepth 2 -maxdepth 2 -type f -name '*.cpp' -print0)

example_count=0
while IFS= read -r -d '' makefile; do
  example_dir="${makefile%/*}"
  sources=("$example_dir"/*.cpp)
  if [[ ! -e "${sources[0]}" ]]; then
    report_error "example Makefile has no C++ source: $makefile"
    continue
  fi

  if ! make --no-print-directory --dry-run -C "$example_dir" all >/dev/null; then
    report_error "example Makefile dry-run failed: $makefile"
  fi
  example_count=$((example_count + 1))
done < <(find examples -mindepth 2 -maxdepth 2 -type f -name Makefile -print0)

if ((example_count == 0)); then
  report_error "no example Makefiles found"
fi

if ! make --no-print-directory --dry-run -C examples all >/dev/null; then
  report_error "aggregate examples/Makefile dry-run failed"
fi

if ((status != 0)); then
  exit "$status"
fi

printf 'Repository hygiene check passed (%d example directories).\n' "$example_count"
