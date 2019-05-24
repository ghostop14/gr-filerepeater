#!/usr/bin/env python
# 

from gnuradio import gr
import pmt

class MetaToPair(gr.sync_block):
  def __init__(self, incomingKeyName, outgoingPairName):
    gr.sync_block.__init__(self, name = "MetaToPair", in_sig = None, out_sig = None)
    
    self.incomingKeyName = str(incomingKeyName)
    self.outgoingPairName = str(outgoingPairName)
         
    self.message_port_register_in(pmt.intern("inmeta"))
    self.set_msg_handler(pmt.intern("inmeta"), self.msgHandler)   
    self.message_port_register_out(pmt.intern("outpair"))

  def msgHandler(self, msg):
    meta = pmt.to_python(pmt.car(msg))
    
    if not type(meta) is dict:
      print "[MetaToPair] ERROR: incoming message does not contain a dictionary."
      return
      
    if not self.incomingKeyName in meta:
      print "[MetaToPair] ERROR: incoming message dictionary does not contain key %s" % self.incomingKeyName
      return
    
    incomingVal = meta[self.incomingKeyName]

    newPair = None
        
    try:  
    	newPair = pmt.cons(pmt.intern(self.outgoingPairName),pmt.to_pmt(incomingVal))
    except Exception as e:
      print "[MetaToPair] ERROR: Cannot construct new message: %s" % str(e)
      return
    
    try:    
      self.message_port_pub(pmt.intern("outpair"),newPair)
    except Exception as e:
      print "[MetaToPair] ERROR: Cannot send message: %s" % str(e)
      print "Incoming dictionary (%s):" % str(type(meta))
      print str(meta)    
    
  def stop(self):
    return True
