die() {

	echo " *** ERROR: " $*

	exit 1 

}
set -x 

if [ "${TRAVIS}" == "true" ] ; then
    [ "${TRAVIS_OS_NAME}" != "linux" ] || {
        sudo apt-get update || die
        sudo apt-get install lib32z1 || die
        sudo apt-get install dos2unix || die
        sudo apt-get install doxygen || die
    }
fi

{
    pip install --upgrade pip || die
    pip install PrettyTable || die
    pip install colorama || die
    pip install configparser || die
}