die() {
    echo " *** ERROR: " $*
    exit 1

}
set -x

[ $TRAVIS_OS_NAME != linux ] || {
    U_NAME=${U_NAME:=embARC_Bot}
    U_EMAIL=${U_EMAIL:=info@embARC.org}
    echo $U_NAME, $U_EMAIL
    git config --global user.name "${U_NAME}"
    git config --global user.email "${U_EMAIL}"

    export PATH=/tmp/arc_gnu_${GNU_VER}_prebuilt_elf32_le_linux_install/bin:$PATH || die
    git checkout -- . || die
    git archive --format zip -o applications.zip --prefix embarc_applications/ HEAD || die
    [ $embARC_OSP_REPO ] || {
        embARC_OSP_REPO="https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git"
    }
    git clone ${embARC_OSP_REPO} embarc_osp
    cd embarc_osp || die
    unzip ../applications.zip || die
    bash apply_embARC_patch.sh || die
    # cd ../ || die
    cd .travis || die
    {
        BUILD_OPTS="OSP_ROOT=${OSP_ROOT} TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE} GNU_VER=${GNU_VER} EXAMPLES=${EXAMPLES}"
        python build.py ${BUILD_OPTS} || die
    }

}
