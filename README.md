# Playerd
- Unofficial Playerd repository, for official visit [here](http://hg.playerd.org/playerd).
- Update: Server hosting the repo is currently down.

## Introduction
This work was done during [Google Summer of Code Internship](https://www.google-melange.com/archive/gsoc/2015/orgs/copyleftgames/projects/avataran_27_3.html). To design Bluetooth wrappers for macOS and Android, I used Delegate design pattern. In macOS Objective-C delegates interact with C code through Objective-C runtime library and in android Java Native Interface is used to bridge Java delegate and C code. This is not updated version and may contain errors. 

## Building and installing it
==========================

run:
```bash
	$ ./waf configure --platform=Platform
	Available platforms: systemd, windows, mac, android

	$ ./waf build
	# ./waf install
```

