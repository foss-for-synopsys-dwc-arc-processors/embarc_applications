die() {
    echo " *** ERROR: " $*
    exit 1

}

TOOLCHAIN_CACHE_FOLDER=".cache/toolchain"
ARC_DEV_GNU_ROOT="/u/arcgnu_verif/gnu_builds"
ARC_DEV_MW_ROOT="/u/relauto/.TOOLS_ROOT/ToolsCommon/MWDT_eng/"

if [ "${TOOLCHAIN}" == "gnu" ]; then
    ARC_DEV_TOOL_ROOT="${ARC_DEV_GNU_ROOT}/${TOOLCHAIN_VER}/elf32_le_linux"
else
    ARC_DEV_TOOL_ROOT="${ARC_DEV_MW_ROOT}/mwdt_${TOOLCHAIN_VER}/linux/ARC"
fi

U_NAME=${U_NAME:=embARC_Bot}
U_EMAIL=${U_EMAIL:=info@embARC.org}
echo $U_NAME, $U_EMAIL
git config --global user.name "${U_NAME}"
git config --global user.email "${U_EMAIL}"
git checkout -- . || die
git archive --format zip -o applications.zip --prefix embarc_applications/ HEAD || die
[ $embARC_OSP_REPO ] || {
    embARC_OSP_REPO="https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git"
}
git clone ${embARC_OSP_REPO} embarc_osp
cd embarc_osp || die
unzip ../applications.zip>/dev/null 2>&1 || die


[ "$TRAVIS" == "true" ] && {
    if [ "${TOOLCHAIN}" == "gnu" ]; then

        python .ci/toolchain.py -v $TOOLCHAIN_VER -c $TOOLCHAIN_CACHE_FOLDER  || die
        if [ -d $TOOLCHAIN_CACHE_FOLDER ] ;then
            if [ -d $TOOLCHAIN_CACHE_FOLDER/$TOOLCHAIN_VER ] ; then
                ARC_DEV_TOOL_ROOT="${TOOLCHAIN_CACHE_FOLDER}/${TOOLCHAIN_VER}"
            fi
        fi
    else
        die "Toolchain not supported in travis ci"
    fi
}

if [ -d $ARC_DEV_TOOL_ROOT ] ; then
    bash .ci/linux_env_set_arc.sh -t $TOOLCHAIN -r $ARC_DEV_TOOL_ROOT || die
    [ ! -e "arctool.env" ] && die "arctool.env doesn't exist"
    source arctool.env || die
    rm -rf arctool.env || die
else
    die "The toolchain path does not exist "
fi


if [ "${TOOLCHAIN}" == "gnu" ]; then
    arc-elf32-gcc -v || die "ARC GNU toolchain is not installed correctly"
else
    ccac -v || die "MWDT toolchain is not installed correctly"
fi


{

    bash apply_embARC_patch.sh || die

    EXPECTED="../${EXPECTED}"
    cd .ci || die
    {
        BUILD_OPTS="OSP_ROOT=${OSP_ROOT} TOOLCHAIN=${TOOLCHAIN} BOARD=${BOARD} BD_VER=${BD_VER} CUR_CORE=${CUR_CORE} GNU_VER=${GNU_VER} EXAMPLES=${EXAMPLES} EXPECTED=${EXPECTED}"
        python build.py ${BUILD_OPTS} || die
    }

}
