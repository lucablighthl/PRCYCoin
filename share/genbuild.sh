#!/bin/sh
# Copyright (c) 2012-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C
if [ $# -gt 1 ]; then
    cd "$2" || exit 1
fi
if [ $# -gt 0 ]; then
    FILE="$1"
    shift
    if [ -f "$FILE" ]; then
        INFO="$(head -n 1 "$FILE")"
    fi
else
    echo "Usage: $0 <filename> <srcroot>"
    exit 1
fi

git_check_in_repo() {
    ! { git status --porcelain -uall --ignored "$@" 2>/dev/null || echo '??'; } | grep -q '?'
}

DESC=""
SUFFIX=""
<<<<<<< HEAD
LAST_COMMIT_DATE=""
if [ "${BITCOIN_GENBUILD_NO_GIT}" != "1" -a -e "$(which git 2>/dev/null)" -a "$(git rev-parse --is-inside-work-tree 2>/dev/null)" = "true" ] && git_check_in_repo share/genbuild.sh; then
    # clean 'dirty' status of touched files that haven't been modified
    git diff >/dev/null 2>/dev/null 

    # if latest commit is tagged and not dirty, then override using the tag name
    RAWDESC=$(git describe --abbrev=0 2>/dev/null)
    if [ "$(git rev-parse HEAD)" = "$(git rev-list -1 $RAWDESC 2>/dev/null)" ]; then
=======
if [ "${BITCOIN_GENBUILD_NO_GIT}" != "1" ] && [ -e "$(command -v git)" ] && [ "$(git rev-parse --is-inside-work-tree 2>/dev/null)" = "true" ] && git_check_in_repo share/genbuild.sh; then
    # clean 'dirty' status of touched files that haven't been modified
    git diff >/dev/null 2>/dev/null

    # if latest commit is tagged and not dirty, then override using the tag name
    RAWDESC=$(git describe --abbrev=0 2>/dev/null)
    if [ "$(git rev-parse HEAD)" = "$(git rev-list -1 "$RAWDESC" 2>/dev/null)" ]; then
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        git diff-index --quiet HEAD -- && DESC=$RAWDESC
    fi

    # otherwise generate suffix from git, i.e. string like "59887e8-dirty"
    SUFFIX=$(git rev-parse --short HEAD)
<<<<<<< HEAD
    git diff-index --quiet HEAD -- || SUFFIX=""

    # get a string like "2012-04-10 16:27:19 +0200"
    LAST_COMMIT_DATE="$(git log -n 1 --format="%ci")"
=======
    git diff-index --quiet HEAD -- || SUFFIX="$SUFFIX-dirty"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
fi

if [ -n "$DESC" ]; then
    NEWINFO="#define BUILD_DESC \"$DESC\""
elif [ -n "$SUFFIX" ]; then
    NEWINFO="#define BUILD_SUFFIX $SUFFIX"
else
    NEWINFO="// No build information available"
fi

# only update build.h if necessary
if [ "$INFO" != "$NEWINFO" ]; then
    echo "$NEWINFO" >"$FILE"
<<<<<<< HEAD
    if [ -n "$LAST_COMMIT_DATE" ]; then
        echo "#define BUILD_DATE \"$LAST_COMMIT_DATE\"" >> "$FILE"
    fi
fi
=======
fi
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
