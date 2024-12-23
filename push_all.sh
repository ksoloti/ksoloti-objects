#!/bin/sh
git checkout 1.1.0
git push
git checkout master
git merge 1.1.0
git push
git checkout 1.1.0
