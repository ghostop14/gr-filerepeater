#!/usr/bin/env python
# 

from gnuradio import gr
import pmt
import sys
import time

class MsgToFile(gr.sync_block):
  def __init__(self, filename, includeTimestamp,overwrite):
    gr.sync_block.__init__(self, name = "Message To File", in_sig = None, out_sig = None)
    
    self.filename = filename
    self.includeTimestamp = includeTimestamp
    
    try:
      # 0 says unbuffered / write immediately
      if (overwrite):
        self.fp = open(filename,"w",0)
      else:
        self.fp = open(filename,"a",0)
    except Exception as e:
      print("[MsgToFile] Error opening file: %s" % str(e))
      sys.exit(1)
     
    self.message_port_register_in(pmt.intern("msg"))
    self.set_msg_handler(pmt.intern("msg"), self.msgHandler)   

  def msgHandler(self, msg):
    if (self.fp):
      if self.includeTimestamp:
        self.fp.write("["+time.strftime('%H:%M:%S %m/%d/%Y')+"]]\n")
        
      self.fp.write(str(msg)+"\n")
    
  def stop(self):
    if (self.fp):
      try:
        self.fp.close()
      except:
        pass
      
    return True
    