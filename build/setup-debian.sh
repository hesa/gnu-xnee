#!/bin/sh

DOC_PKGS="texlive-extra-utils  libxtst-dev gs-common texi2html texinfo  imagemagick pkg-config dia  texi2html texlive-font-utils" 

DEVEL_PKGS="libtool gcc automake autoconf make libgtk2.0-dev"

my_install() 
{
    apt-get install -y  $*
}


my_install $DOC_PKGS

my_install $DEVEL_PKGS

