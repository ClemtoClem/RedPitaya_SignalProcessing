#!/bin/bash
# Executer le script envoyer pour le code modules.hpp et modules.cpp à la RedPitaya

scp -r ./src/modules.* root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing/src/