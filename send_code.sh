#!/bin/bash
# Executer le script envoyer le contenue du réêrtoire ./src à la RedPitaya

scp -r ./src/* root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing/src/