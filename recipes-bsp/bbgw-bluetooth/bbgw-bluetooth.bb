SUMMARY = "BBGW wireless support"

LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://README.md;md5=7f4097269f3e3b1c5a99047d07365771"

SRC_URI = "file://bb-wl18xx-bluetooth \
           file://bb-wl18xx-bluetooth.service \
           file://TIInit_11.8.32.bts \
           file://README.md \
          "

PR = "r0"

S = "${WORKDIR}"

inherit systemd


do_install_append () {
    install -d ${D}${systemd_unitdir}/system
    install -m 0755 bb-wl18xx-bluetooth.service ${D}${systemd_unitdir}/system

    install -d ${D}/lib/firmware/ti-connectivity
    install -m 0644 TIInit_11.8.32.bts ${D}/lib/firmware/ti-connectivity

    install -d ${D}/usr/bin
    install -m 0755 bb-wl18xx-bluetooth ${D}/usr/bin

}

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "bb-wl18xx-bluetooth.service"

FILES_${PN} = "${sysconfdir} /lib/firmware/ti-connectivity ${systemd_unitdir} /usr/bin"
