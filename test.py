#!/usr/bin/python

import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade

class TestGtk:
	def __init__(self):
		self.gladefile = "ui.glade"
		self.wTree = gtk.glade.XML(self.gladefile)
		self.window = self.wTree.get_widget("main_window")
		self.window.connect("destroy", gtk.main_quit)
		self.window.show()
	def main(self):
		gtk.main()

if __name__ == "__main__":
	test = TestGtk()
	test.main()
