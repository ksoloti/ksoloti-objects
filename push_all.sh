#!/bin/sh
git checkout 1.0.12
git push
git checkout master
git merge 1.0.12
git push
git checkout 1.0.12