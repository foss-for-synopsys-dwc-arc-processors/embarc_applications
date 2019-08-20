#!/bin/sh

die()
{
    echo " *** ERROR: " $*
    exit 1
}

# When pull request, check code style
if [ "$TRAVIS_PULL_REQUEST" != "false" ]; then
    git diff --name-only --diff-filter=d FETCH_HEAD..master \
        | ( grep '.\(c\|cpp\|h\|hpp\)$' || true ) \
        | while read file; do cpplint "${file}" >> result.log 2>&1; done
    

    git diff --name-only --diff-filter=d FETCH_HEAD..master \
        | ( grep '.\(py\)$' || true ) \
        | while read file; do flake8 "${file}" >> result.log 2>&1; done


    COMMENT_CONTENT=$(sed 's/$/&<br>/g' result.log)
    COMMENT_HEAD="# Code style check result \n***********************\n<pre>"
    COMMENT_TAIL="</pre>"
    COMMENT="${COMMENT_HEAD}${COMMENT_CONTENT}${COMMENT_TAIL}"
    bash -c "$COMMENTS"
fi
