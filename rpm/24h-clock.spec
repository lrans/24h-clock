%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Name:       24h-clock
Summary:    24H clock
Version:    1.0.0
Release:    1
Group:      Qt/Qt
License:    BSD
URL:        https://github.com/lrans/24h-clock
Source0:    %{name}-%{version}.tar.bz2

Requires:      sailfishsilica-qt5 >= 0.10.9
Requires:      qt5-qtdeclarative-import-positioning
BuildRequires: pkgconfig(sailfishapp) >= 1.0.2
BuildRequires: pkgconfig(mlite5)
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: desktop-file-utils
BuildRequires: qt5-qttools-linguist
BuildRequires: qt5-qtdeclarative-import-positioning

%description
24H clock with sunrise/sunset (forked from https://github.com/monich/harbour-swissclock)

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png

%changelog
* Tue Aug 18 2015 Snarl <lrans@users.noreply.github.com> 1.0.0
- Initial version (forked from https://github.com/monich/harbour-swissclock)
