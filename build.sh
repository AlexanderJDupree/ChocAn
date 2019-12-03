#!/usr/bin/env bash
#
# Optional build script to build and run project and unit tests.
# To execute run:
#   $ ./build.sh
#
# Alternatively, you can manually build the project by running the following in
# 'gmake' directory
#
#   $ make all
#
# TODO Add check to ensure GCC version is >= 7.0
# TODO If lib-bash fails we should build manually with make -C gmake

VERBOSE=""
COMPILER="g++"

( [[ -n ${ZSH_EVAL_CONTEXT} && ${ZSH_EVAL_CONTEXT} =~ :file$ ]] || \
  [[ -n $BASH_VERSION && $0 != "$BASH_SOURCE" ]]) && _s_=1 || _s_=0

export _s_
export ProjectRoot=$(pwd)
export BuildDir="${ProjectRoot}/build"
export BashLibRoot="${ProjectRoot}/bin/lib-bash"
export LibBashRepo="https://github.com/kigster/lib-bash"

# We are using an awesome BASH library `lib-bash` for prettifying the output, and
# running commands through their LibRun framework.

check_cpp17_compatible(){

    CPP_REQUIRED=7
    version=$(g++ -dumpversion)
    CPP_VERSION=( ${version//./ } ) # replace points, split into array

    if [ "${CPP_VERSION[0]}" -lt "$CPP_REQUIRED" ] && [ $COMPILER == "g++" ]
    then
        echo "ERROR: ChocAn requires g++ version" $CPP_REQUIRED  "or greater to support c++ standard 17."
        echo "current version: " $CPP_VERSION
        echo "required version: " $CPP_REQUIRED
        echo "Please update and try again."
        exit 1
    fi

}

build_alternate() {
  cd gmake/
  echo && make -j 12 config=debug ${VERBOSE}
  echo && make -j 12 config=release ${VERBOSE}
  cd ${ProjectRoot}

  if [[ -d bin/tests/ ]]; then

    echo && bin/tests/debug_tests

    echo && bin/tests/release_tests

  else
    echo "Can't find Test executable in chocan/bin/tests"
    exit 2
  fi

  exit 0
}

lib-bash() {
  [[ ! -d ${BashLibRoot} ]] && curl -fsSL https://git.io/fxZSi | /usr/bin/env bash
  [[ ! -d ${BashLibRoot} ]] && {
    printf "Unable to git clone lib-bash repo from ${LibBashRepo}"
    printf "\n\nRunning Alternate Build Process"
    build_alternate
  }

  if [[ -f ${BashLibRoot}/Loader.bash ]]; then
    cd ${BashLibRoot} > /dev/null
    git reset --hard origin/master 2>&1 | cat >/dev/null
    git pull 2>&1 | cat >/dev/null
    [[ -f Loader.bash ]] && source Loader.bash
    cd ${ProjectRoot}
  else
    printf "\nERROR: unable to find lib-bash library from ${LibBashRepo}!\n"
    printf "\n\nRunning Alternate Build Process"
    build_alternate
  fi

  run::set-all show-output-off abort-on-error
}

header() {
  h1::purple "CS 300 Term Project : ChocAn Data Processing Application"
  local OIFC=${IFC}
  IFS="|" read -r -a gcc_info <<< "$(gcc --version 2>&1 | tr '\n' '|')"
  export IFC=${OIFC}
  h1 "${bldylw}GCC:" "${gcc_info[0]}" "${gcc_info[1]}" "${gcc_info[2]}" "${gcc_info[3]}" "${gcc_info[4]}"
  h1 "${bldylw}GIT:    ${bldblu}$(git --version)"


  local OIFC=${IFC}
  IFS="|" read -r -a make_info <<< "$(make --version 2>&1 | tr '\n' '|')"
  export IFC=${OIFC}

  h1 "${bldylw}MAKE:" "${make_info[0]}"
}

build() {

  h2 "Building Debug Configuration"

  run "cd gmake"

  run::set-next show-output-on
  run "echo && make -j 12 config=debug ${VERBOSE}"

  h2 "Building Release Configuration"

  run::set-next show-output-on
  run "echo && make -j 12 config=release ${VERBOSE}"

  run "cd ${ProjectRoot}"
}

tests() {
  if [[ -d bin/tests/ ]]; then

    h2 "Running Tests"

    hl::subtle "Debug Configuration Tests"

    run::set-next show-output-on
    run "echo && bin/tests/debug_tests"

    hl::subtle "Release Configuration Tests"

    run::set-next show-output-on
    run "echo && bin/tests/release_tests"

  else
    printf "${bldred}Can't find installed executable ${bldylw}bin/tests/.${clr}\n"
    exit 2
  fi
  h::green "Build Complete!"
  echo " "
}

usage() {
  printf "\nChocAn Build Script:\n\nBuilds the ChocAn Application and executes unit tests\n"
  printf "\nusage:\n\tbuild.sh  options\n"
  printf "\nwhere options are:\n"
  printf "\t-?, -h, --help\t\tdisplay usage information\n"
  printf "\t-v, --verbose\t\tBuild in verbose mode\n"
}

main() {


  check_cpp17_compatible
  lib-bash
  header
  build
  tests
}

while [ "$1" != "" ]; do
    case $1 in
        -v | --verbose )        VERBOSE="verbose=true"
                                ;;
        -c | --clang )          COMPILER="clang"
                                ;;
        -h | --help | -? )      usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done

(( $_s_ )) || main
