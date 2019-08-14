#!/bin/sh

die()
{
    echo " *** ERROR: " $*
    exit 1
}

# When pull request, check code style
if [ "$TRAVIS_PULL_REQUEST" != "false" ] ; then
    c_style_check=$(git diff --name-only --diff-filter=d FETCH_HEAD..HEAD \
                | ( grep '.\(c\|cpp\|h\|hpp\)$' || true ) \
                | while read file; do cpplint "${file}"; done)
    python_style_check=$(git diff --name-only --diff-filter=d FETCH_HEAD..HEAD \
                | ( grep '.\(py\)$' || true ) \
                | while read file; do flake8 "${file}"; done)

    COMMENT_HEAD="## Code Style Check Result \n***********************\n<pre>"
    COMMENT_TAIL="</pre>"
    COMMENT="${COMMENT_HEAD}${c_style_check}\n${python_style_check}${COMMENT_TAIL}"
    bash -c "$COMMENTS"
    exit 0
fi
exit 0
