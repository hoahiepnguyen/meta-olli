SRC_URI_append_keystone = " file://keystone-uio.dtsi"
SRC_URI_append_k2hk = " file://k2hk-uio.dtsi"
SRC_URI_append_k2l-evm = " file://k2l-uio.dtsi"
SRC_URI_append_k2e = " file://k2e-uio.dtsi"
SRC_URI_append_am57xx-evm = " file://am572x-pru-uio.dtsi"
SRC_URI_append_ti33x = " file://am335x-pru-uio.dtsi"
SRC_URI_append_ti43x = " file://am437x-pru-uio.dtsi"

DTSI_LIST = ""
DTSI_LIST_append_keystone = " keystone-uio.dtsi"
DTSI_LIST_append_k2hk = " k2hk-uio.dtsi"
DTSI_LIST_append_k2l-evm = " k2l-uio.dtsi"
DTSI_LIST_append_k2e = " k2e-uio.dtsi"

KERNEL_DEVICETREE_append_am57xx-evm = "${@base_conditional("ENABLE_TI_UIO_DEVICES", "1", " am572x-idk-pru-excl-uio.dtb", "", d)}"
KERNEL_DEVICETREE_append_ti33x = "${@base_conditional("ENABLE_TI_UIO_DEVICES", "1", " am335x-icev2-pru-excl-uio.dtb", "", d)}"
KERNEL_DEVICETREE_append_ti43x = "${@base_conditional("ENABLE_TI_UIO_DEVICES", "1", " am437x-idk-pru-excl-uio.dtb", "", d)}"

do_setup_uio() {
    if [ "${ENABLE_TI_UIO_DEVICES}" == "1" ]
    then
        for dtsi in ${DTSI_LIST}
        do
            cp ${WORKDIR}/${dtsi} ${S}/arch/arm/boot/dts/
            for dtb in ${KERNEL_DEVICETREE}
            do
                dts=`echo $dtb | sed -e 's|dtb$|dts|'`
                echo "#include \"${dtsi}\"" >> ${S}/arch/arm/boot/dts/$dts
            done
        done
    fi
}

do_setup_uio_append_am57xx-evm() {

    if [ "${ENABLE_TI_UIO_DEVICES}" == "1" ]
    then
        dts="am572x-idk-pru-excl-uio.dts"
        dtsi="am572x-pru-uio.dtsi"
        cp ${S}/arch/arm/boot/dts/am572x-idk.dts ${S}/arch/arm/boot/dts/$dts
        cp ${WORKDIR}/${dtsi} ${S}/arch/arm/boot/dts/
        echo "#include \"${dtsi}\"" >> ${S}/arch/arm/boot/dts/$dts
    fi
}

do_setup_uio_append_ti33x() {

    if [ "${ENABLE_TI_UIO_DEVICES}" == "1" ]
    then
        dts="am335x-icev2-pru-excl-uio.dts"
        dtsi="am335x-pru-uio.dtsi"
        cp ${S}/arch/arm/boot/dts/am335x-icev2.dts ${S}/arch/arm/boot/dts/$dts
        cp ${WORKDIR}/${dtsi} ${S}/arch/arm/boot/dts/
        echo "#include \"${dtsi}\"" >> ${S}/arch/arm/boot/dts/$dts
    fi
}

do_setup_uio_append_ti43x() {

    if [ "${ENABLE_TI_UIO_DEVICES}" == "1" ]
    then
        dts="am437x-idk-pru-excl-uio.dts"
        dtsi="am437x-pru-uio.dtsi"
        cp ${S}/arch/arm/boot/dts/am437x-idk-evm.dts ${S}/arch/arm/boot/dts/$dts
        cp ${WORKDIR}/${dtsi} ${S}/arch/arm/boot/dts/
        echo "#include \"${dtsi}\"" >> ${S}/arch/arm/boot/dts/$dts
    fi
}

do_patch[postfuncs] += "do_setup_uio"
