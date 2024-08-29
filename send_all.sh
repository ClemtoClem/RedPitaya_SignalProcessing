#!/bin/bash
# Executer le script envoyer le contenue du répertoire à la RedPitaya

scp -r ./src root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing/
scp Makefile root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing/
scp -r ./compile.sh root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing/
