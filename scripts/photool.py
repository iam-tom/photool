#!/usr/bin/python

import os
import sys
import shutil
import subprocess
import time
from datetime import date


class Photool:
  def __init__(self):
    self.init_msg()
    # path definitions
    root_dir=os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
    binary_dir="bin"
    binary_prefix="photool-"
    self.binaries={ "photool-importer":os.path.join(root_dir,binary_dir,binary_prefix+"importer"),
                    "photool-phoview":os.path.join(root_dir,binary_dir,binary_prefix+"phoview"),
                    "photool-copy2dev":os.path.join(root_dir,binary_dir,binary_prefix+"copy2dev"),
                    "photool-phosort":os.path.join(root_dir,binary_dir,binary_prefix+"phosort")}
    binaries_valid =self.verify_binaries()

    if( binaries_valid== False):
      print "ERROR: not all binaries exist - build photool project first - or check path definitions in script"

   # None initializations
    self.dst_dir=None
    self.src_dir=None
    self.dst_snapshot_dir=None
    self.dst_dev_dir=None


  def init_msg(self):
    print "************************************************"
    print "Photool - 2013-12-25                            "
    print "************************************************"


  def verify_binaries(self):
    binaries_valid=True
    binaries_valid=os.path.exists(self.binaries["photool-importer"])
    if(binaries_valid==False): return False
    binaries_valid=os.path.exists(self.binaries["photool-copy2dev"])
    if(binaries_valid==False): return False
    binaries_valid=os.path.exists(self.binaries["photool-phoview"])
    if(binaries_valid==False): return False
    binaries_valid=os.path.exists(self.binaries["photool-phosort"])
    if(binaries_valid==False): return False
    return True

  def set_dst_dir(self,dst_dir):
    self.dst_dir=dst_dir
    self.dst_snapshot_dir=os.path.join(dst_dir,"snapshot")
    self.dst_dev_dir=os.path.join(dst_dir,"dev")

  def set_src_dir(self,src_dir):
    self.src_dir=src_dir

  def run_import(self):
    if(self.dst_dir and self.src_dir):
      print "--> running import "
      print "From : %s"%self.src_dir
      print "To   : %s"%self.dst_dir
      answer=raw_input("performing operarion (y/n)")
      if(answer is "y"):
        if not os.path.exists(self.dst_dir): os.makedirs(self.dst_dir)
        subprocess.call([self.binaries["photool-importer"],self.src_dir,self.dst_dir])
      else:
        print "Import cancelled"
    else:
      print "ERROR: set source and destination directories first"

  def run_viewer(self):
      answer=raw_input("view snapshots (y/n)")
      if(answer is "y"):
        subprocess.call([self.binaries["photool-phoview"],self.dst_snapshot_dir])

  def run_filter(self,val):
      answer=raw_input("copy images rated 3 to dev folder  (y/n)")
      if(answer is "y"):
        subprocess.call([self.binaries["photool-copy2dev"],self.dst_dev_dir])






if __name__=="__main__":
  dst_dir="/media/Data/MEDIA/photography/"
  src_dir=os.getcwd()

  pt=Photool()
  if(len(sys.argv)==1):
    date=date.today()
    date=str(date)
    dst_dir=os.path.join(dst_dir,date)
    #pt.set_target_dir
  elif(len(sys.argv)==2):
    dst_dir=sys.argv[1]
    #dst_dir=os.path.join(dst_dir,sys.argv[1])

  pt.set_dst_dir(dst_dir)
  pt.set_src_dir(src_dir)
  pt.run_import()

  pt.run_viewer()
  pt.run_filter(3)






