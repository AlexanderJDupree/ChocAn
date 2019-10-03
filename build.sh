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


( [[ -n ${ZSH_EVAL_CONTEXT} && ${ZSH_EVAL_CONTEXT} =~ :file$ ]] || \
  [[ -n $BASH_VERSION && $0 != "$BASH_SOURCE" ]]) && _s_=1 || _s_=0

export _s_
export ProjectRoot=$(pwd)
export BuildDir="${ProjectRoot}/build"
export BashLibRoot="${ProjectRoot}/bin/lib-bash"
export LibBashRepo="https://github.com/kigster/lib-bash"

# We are using an awesome BASH library `lib-bash` for prettifying the output, and
# running commands through their LibRun framework.
lib-bash() {
  [[ ! -d ${BashLibRoot} ]] && curl -fsSL https://git.io/fxZSi | /usr/bin/env bash
  [[ ! -d ${BashLibRoot} ]] && { 
    printf "Unable to git clone lib-bash repo from ${LibBashRepo}"
    exit 1
  }
  
  if [[ -f ${BashLibRoot}/Loader.bash ]]; then
    cd ${BashLibRoot} > /dev/null
    git reset --hard origin/master 2>&1 | cat >/dev/null
    git pull 2>&1 | cat >/dev/null
    [[ -f Loader.bash ]] && source Loader.bash
    cd ${ProjectRoot}
  else
    printf "\nERROR: unable to find lib-bash library from ${LibBashRepo}!\n"
    exit 1
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
  run "echo && make -j 12 config=debug"

  h2 "Building Release Configuration"

  run::set-next show-output-on
  run "echo && make -j 12 config=release"

  run "cd ${ProjectRoot}"
}

tests() {
  if [[ -d bin/tests/ ]]; then

    hl::subtle "Running Debug Configuration Tests"

    run::set-next show-output-on
    run "echo && bin/tests/debug_tests"

    hl::subtle "Running Release Configuration Tests"

    run::set-next show-output-on
    run "echo && bin/tests/release_tests"

  else
    printf "${bldred}Can't find installed executable ${bldylw}bin/tests/.${clr}\n"
    exit 2
  fi
}


main() {
  lib-bash
  header
  build
  tests
  h::green "Build Complete!"
}

(( $_s_ )) || main
