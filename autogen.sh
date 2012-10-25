#!/bin/sh -e
${INTLTOOLIZE:-intltoolize} -f -c --automake
${AUTORECONF:-autoreconf} -fiv
