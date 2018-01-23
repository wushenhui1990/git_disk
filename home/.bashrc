#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return
HISTSIZE=2000000
HISTFILESIZE=2000000

PATH=~/git_disk/bin:~/git_disk/sh:$PATH
alias ls='ls --color=auto'
alias grep='grep --color=auto'
alias fgrep='fgrep --color=auto'
alias egrep='egrep --color=auto'
alias svndiff='svn diff|colordiff|less -r'
PS1='\w\$ '
cd X1000/platform
source tools/build/source.sh
