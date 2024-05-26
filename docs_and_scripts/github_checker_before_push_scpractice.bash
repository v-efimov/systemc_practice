#!/usr/bin/env bash

set -Eeuo pipefail  # See the meaning in README.md
# set -x  # Print each command

#-----------------------------------------------------------------------------

script_path="$0"
script=$(basename "$script_path")
script_dir=$(dirname "$script_path")

run_dir="$PWD"
cd "$script_dir/systemc_practice"

pkg_src_root=$(readlink -e .)
pkg_src_root_name=$(basename "$pkg_src_root")

#-----------------------------------------------------------------------------

error ()
{
    printf "\n$script: ERROR: $*\n" 1>&2
    exit 1
}

#-----------------------------------------------------------------------------

f=$(git diff --name-status --diff-filter=R HEAD .)

if [ -n "${f-}" ]
then
    error "there are renamed files in the tree."                            \
          "\nYou should check them in before preparing a release package."  \
          "\nSpecifically:\n\n$f"
fi

f=$(git ls-files --others --exclude-standard .)

if [ -n "${f-}" ]
then
    error "there are untracked files in the tree."          \
          "\nYou should either remove or check them in"     \
          "before preparing a release package."             \
          "\nSpecifically:\n\n$f"                           \
          "\n\nYou can also see the file list by running:"  \
          "\n    git clean -d -n $pkg_src_root"             \
          "\n\nAfter reviewing (be careful!),"              \
          "you can remove them by running:"                 \
          "\n    git clean -d -f $pkg_src_root"             \
          "\n\nNote that \"git clean\" does not see"        \
          "the files from the .gitignore list."
fi

f=$(git ls-files --others .)

if [ -n "${f-}" ]
then
    error "there are files in the tree, ignored by git,"                    \
          "based on .gitignore list."                                       \
          "\nThis repository is not supposed to have the ignored files."    \
          "\nYou need to remove them before preparing a release package."   \
          "\nSpecifically:\n\n$f"
fi

f=$(git ls-files --modified .)

if [ -n "${f-}" ]
then
    error "there are modified files in the tree."                           \
          "\nYou should check them in before preparing a release package."  \
          "\nSpecifically:\n\n$f"
fi

#-----------------------------------------------------------------------------

# Search for the text files with DOS/Windows CR-LF line endings

# -r     - recursive
# -l     - file list
# -q     - status only
# -I     - Ignore binary files
# -U     - don't strip CR from text file by default
# $'...' - string literal in Bash with C semantics ('\r', '\t')

if [ "$OSTYPE" = linux-gnu ] && grep -rqIU $'\r$' ./*
then
    grep -rlIU $'\r$' ./*

    error "there are text files with DOS/Windows CR-LF line endings." \
          "You can fix them by doing:" \
          "\ngrep -rlIU \$'\\\\r\$' \"$pkg_src_root\"/* | xargs dos2unix"
fi

if grep -rqI $'\t' ./*
then
    grep -rlI $'\t' ./*

    error "there are text files with tabulation characters." \
          "\nTabs should not be used." \
          "\nDevelopers should not need to configure the tab width" \
          " of their text editors in order to be able to read source code." \
          "\nPlease replace the tabs with spaces" \
          "before checking in or creating a package." \
          "\nYou can find them by doing:" \
          "\ngrep -rlI \$'\\\\t' \"$pkg_src_root\"/*" \
          "\nYou can fix them by doing the following," \
          "but make sure to review the fixes:" \
          "\ngrep -rlI \$'\\\\t' \"$pkg_src_root\"/*" \
          "| xargs sed -i 's/\\\\t/    /g'"
fi

if grep -rqI '[[:space:]]\+$' ./*
then
    grep -rlI '[[:space:]]\+$' ./*

    error "there are spaces at the end of line, please remove them." \
          "\nYou can fix them by doing:" \
          "\ngrep -rlI '[[:space:]]\\\\+\$' \"$pkg_src_root\"/*" \
          "| xargs sed -i 's/[[:space:]]\\\\+\$//g'"
fi
