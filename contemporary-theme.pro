TEMPLATE = subdirs

themeProj.subdir = theme

demoProj.subdir = demo
demoProj.depends = themeProj

SUBDIRS += themeProj demoProj
