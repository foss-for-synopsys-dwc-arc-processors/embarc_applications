die() {

	echo " *** ERROR: " $*

	exit 1 

}
set -x 

[ $TRAVIS_OS_NAME != linux ] || {

    sudo apt-get update || die 
    sudo apt-get install lib32z1 || die
    sudo apt-get install dos2unix || die
    pip install --upgrade pip || die
    pip install PrettyTable || die
    pip install colorama || die
}
