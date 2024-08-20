#!/bin/bash
# Executer le script envoyer le code tests.hpp et test.cpp à la RedPitaya

scp -r ./src/tests.* root@rp-f0baa5.local:/root/Programs/RedPitaya_SignalProcessing /src/