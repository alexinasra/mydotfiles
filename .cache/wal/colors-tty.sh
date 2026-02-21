#!/bin/sh
[ "${TERM:-none}" = "linux" ] && \
    printf '%b' '\e]P0000101
                 \e]P100637C
                 \e]P200718E
                 \e]P3017FA0
                 \e]P401809F
                 \e]P5018CB0
                 \e]P6009CC4
                 \e]P777cbe0
                 \e]P8538e9c
                 \e]P900637C
                 \e]PA00718E
                 \e]PB017FA0
                 \e]PC01809F
                 \e]PD018CB0
                 \e]PE009CC4
                 \e]PF77cbe0
                 \ec'
