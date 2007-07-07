Summary: Minimal Httpd Library
Name: mihl
Version: 0
Release: 2
%define src %{name}
License: BSD
Group: HTTP Servers 
URL: http://mihl.sourceforge.net/
Prefix: %{_prefix}
Buildroot: %{_tmppath}/%{name}

%description
MIHL is a C library that helps to implement an HTTP embedded server. 
It is designed to be small and to provide only a minimal set of functionalities. 
Typically, MIHL can be used in an embedded device where a Web-based interface is useful 
(configuration, status, etc.). MIHL C library can be compiled for both Linux (x86, PowerPC, ARM) and XP. 
MIHL does not use threads, but rather use non blocking sockets I/O.

%prep
#

%build
#

%install
#

%files
%attr(755,root,root) %{_libdir}/*

%clean
rm -rf $RPM_BUILD_ROOT

%changelog

