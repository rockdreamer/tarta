Name:           kcgui
Version: 0.0.1
Release:        0
Epoch:          0
Summary:        KeyCluster Cross-Platform Monitoring GUI

Group:          Applications/System
License:        GPL
URL:            http://www.keycluster.com
Source0:        kcgui-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
kcgui is a cross-platform monitoring and configuration GUI for 
KeyCluster based cluster SIDs.
Capabilities include log monitoring, service monitoring and control,
configuration management.
Contact Babel srl for more info

# -----------------------------------------------------------------------------

%prep
%setup -q

# -----------------------------------------------------------------------------

%build
[ -n "$QTDIR" ] || . %{_sysconfdir}/profile.d/qt.sh
qmake kcgui.pro
make %{?_smp_mflags}
strip kcgui

# -----------------------------------------------------------------------------

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/opt/%{name}
mkdir -p $RPM_BUILD_ROOT/usr/share/applications
cp kcgui $RPM_BUILD_ROOT/opt/%{name}/
cp kcgui.desktop $RPM_BUILD_ROOT/usr/share/applications/


# -----------------------------------------------------------------------------

%clean
rm -rf $RPM_BUILD_ROOT

# -----------------------------------------------------------------------------

%files 
%defattr(-,root,root,-)
%doc COPYING README.linux
/opt/%{name}/*
%attr(0644,root,root) %{_datadir}/applications/%{name}.desktop

# -----------------------------------------------------------------------------

%changelog
* Mon Jan 09 2006 Claudio Bantaloukas <cbantaloukas@babel.it> - epoch:version-release
- Initial RPM release.

