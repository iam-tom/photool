
import os
import sys
import shutil
import subprocess




if __name__=="__main__":

  supported_types=[".JPG",".jpg",".PNG",".png"]

  root_dir="/home/tom/workspace_cpp/photo/"
  cwd=os.getcwd()

  edit_dir=(os.path.join(cwd,"EDIT"))
  if not os.path.isdir(edit_dir):
    os.mkdir(edit_dir)

  file_list=os.listdir(".")
  img_list=list()

  for file in file_list:
    if os.path.splitext(file)[1] in supported_types:
      img_list.append(file)

  print img_list

  for img in img_list:
    imgname=os.path.splitext(img)[0]
    outputfile=os.path.join(edit_dir,img)

    subprocess.Popen([os.path.join(root_dir,"bin/photo_app"),os.path.join(cwd,img),outputfile,str(2000)])

