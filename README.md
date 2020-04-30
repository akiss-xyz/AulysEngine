[//]: # (Written by noidawt@gmail.com)
[//]: # (Date: 19.11.2019)

Aulys - a barebones cross-platform 3D engine for use in non-Euclidean geometries.
=================================================================================

The name comes from a boy's name meaning "willing". The name is often shortened to Au, for 
'gold' - I know, pretentious, but I think it's kinda neat.

I'd love it if you contributed, and don't hesitate to get in touch - just send me an email at 
noidawt@gmail.com (or you could get in touch on Discord, just ask).

!!! For those looking to run/build this project on Windows
==========================================================
You'll likely have to clone this repository with git - you can't just download the zip file.

Follow the guide here: https://help.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository, and before step 6, where you type 'git clone', you need to type into your command line:
`git config --global core.autocrlf true`
You can install git from https://git-scm.com/download/win.

Here's the explanation:
If unusual errors come up on Windows even though you have the executable in the correct place, you'll have to clone the repository with git - this is because the repository uses UNIX style line endings, and we need to correct these so that you can use this project on Windows. For more details, see https://help.github.com/en/github/using-git/configuring-git-to-handle-line-endings

How to run this project
=======================
The project folder (as well as the "Releases" tab) contains executables for Windows (SandboxDistWin64.exe) and Linux (SandboxDistLinux64). 
These have been built with the PROJECT_ROOT set to nothing - therefore they'll search for assets from the directory they're in. To run these, run them from the folder that this README.md file is in - meaning you'll have to copy the executable for your platform (e.g. SandboxWin64.exe) to this folder if it's not already there.

Building
========

This project is configured using premake5. Get the release for your platform 
[here](https://premake.github.io/download.html#v5), and place it in the project folder.

You might have to update the submodules. Open up a terminal and run 'git submodule update --init -- 
recursive'.

Also, don't forget to set your PROJECT_ROOT path in AulysConf.h correctly so that runtime paths to resources can be resolved.
If you set it to nothing (e.g. ""), the application will search for assets from its own folder.

Windows
-------
- 0: Clone this repository. On Windows, you'll need to make sure your line endings are set 
  correctly, see above.

- 1: Copy the utilities you need out of the utilities folder for easy access:

In the 'utilities' folder, you're likely gonna want the utilites for your platform.

- 2: Download the submodules:
You need to, in your command line for git, do: `git submodule update --init --recursive`.

- 3: Run the generate ('generatevs2019solution.cmd') script we copied out from the utilites folder:

Making sure 'premake5.exe' is available in the folder, just double click the script and it'll set up 
the project for you.

- 4: Make sure the precompiled header (pch.h & pch.cpp) work:

To do this, right click on the Aulys project in the Solution Explorer, and go into the properties. On the left, you'll find the Configuration Properties - under C/C++, click on 'Precompiled Header'. 3 options should come up on the right.
Set the first option, 'Precompiled Header' to 'Create (\Yc)'. Then, make sure that the 'Precompiled Header File' is set to 'src/pch.h'.

- 5: Set the start-up project:

Close the property pages from before. In the Solution Explorer, right-click on "Solution 'Aulys'", and open the Properties. On the left, under 'Common Properties' > 'Startup Project', select the 'Single startup project' radio button and select 'Sandbox'.

- Happy hacking!
Now Visual Studio understands everything about the project!

Linux
-----

- 0: Clone this repository. You might want to check your line endings are set correctly, just in 
  case you commit some changes.
  (https://help.github.com/en/github/using-git/configuring-git-to-handle-line-endings)

- 1: Download the submodules:
You need to, in your command line for git, do: `git submodule update --init --recursive`.

- 2: Copy the utilities you need out of the utilities folder for easy access:

In the 'utilities' folder, you're likely gonna want the utilites for your platform.

- 3: Make sure you have all the requirements.

We need: a compiler, premake5, and make (get it from your package manager, e.g. `sudo apt-get 
install make`).

- Happy hacking!
You can use the run.sh, build.sh, and buildrun.sh scripts as shortcuts now!
