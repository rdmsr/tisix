DEFAULT_TARGET=all

all:
	emacs --script publish.el
	cp -r style assets docs
