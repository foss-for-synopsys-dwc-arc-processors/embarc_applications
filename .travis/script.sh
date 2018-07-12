die() {
    echo " *** ERROR: " $*
    exit 1

}
set -x

[ $TRAVIS_OS_NAME != linux ] || {

    export PATH=/tmp/arc_gnu_2017.09_prebuilt_elf32_le_linux_install/bin:$PATH || die
    git checkout -- . || die
    git archive --format zip -o applications.zip --prefix embarc_applications/ HEAD || die
    [ $embARC_OSP_REPO ] || {
        embARC_OSP_REPO="https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git"
    }
    git clone ${embARC_OSP_REPO} embarc_osp
    cd embarc_osp || die
    unzip ../applications.zip || die
    bash apply_embARC_patch.sh || die
    cd ../ || die
    cd .travis || die
    [ $TOOLCHAIN != gnu -o $BOARD != emsk -o $BD_VER != 11 -o $CUR_CORE != arcem6 ] || {

        python3 build.py "TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE}" || die
    }
    [ $TOOLCHAIN != gnu -o $BOARD != emsk -o $BD_VER != 22 -o $CUR_CORE != arcem7d ] || {

        python3 build.py "TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE}" || die
    }

    [ $TOOLCHAIN != gnu -o $BOARD != emsk -o $BD_VER != 22 -o $CUR_CORE != arcem11d ] || {

        python3 build.py "TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE}" || die
    }
    [ $TOOLCHAIN != gnu -o $BOARD != emsk -o $BD_VER != 23 -o $CUR_CORE != arcem7d ] || {

        python3 build.py "TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE}" || die
    }
    [ $TOOLCHAIN != gnu -o $BOARD != emsk -o $BD_VER != 23 -o $CUR_CORE != arcem11d ] || {

        python3 build.py "TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE}" || die
    }

}
