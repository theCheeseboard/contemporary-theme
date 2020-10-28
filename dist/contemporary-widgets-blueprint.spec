Name:           contemporary-widgets
Version:        1.7
Release:        1%{?dist}
Summary:        Contemporary Widget Theme

License:        GPLv3+
URL:            https://github.com/vicr123/contemporary-theme
Source0:        https://github.com/vicr123/contemporary-theme/archive/v%{version}.tar.gz

%if 0%{?fedora} == 32
BuildRequires:  qt5-devel the-libs-blueprint-devel
Requires:       qt5 the-libs-blueprint
%endif

%if 0%{?fedora} >= 33
BuildRequires:  qt5-qtbase-devel the-libs-blueprint-devel
Requires:       qt5-qtbase the-libs-blueprint
%endif

%define debug_package %{nil}
%define _unpackaged_files_terminate_build 0

%description
Contemporary Widget Theme

%prep
%setup -n contemporary-widgets-%{version}

%build
qmake-qt5
make

%install
rm -rf $RPM_BUILD_ROOT
#%make_install
make install INSTALL_ROOT=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/*


%changelog
* Sun May  3 2020 Victor Tran
- 
