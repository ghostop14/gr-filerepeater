#!/usr/bin/env python
# 

from gnuradio import gr
import pmt

class MetaToPair(gr.sync_block):
  def __init__(self, incomingKeyName, outgoingPairName):
    gr.sync_block.__init__(self, name = "MetaToPair", in_sig = None, out_sig = None)
    
    self.incomingKeyName = incomingKeyName
    self.outgoingPairName = outgoingPairName
         
    self.message_port_register_in(pmt.intern("inmeta"))
    self.set_msg_handler(pmt.intern("inmeta"), self.msgHandler)   
    self.message_port_register_out(pmt.intern("outpair"))

  def msgHandler(self, msg):
    meta = pmt.to_python(pmt.car(pdu))
    
    try:    
      incomingVal = meta[self.incomingKeyName]
      
      self.message_port_pub(pmt.intern("outpair"),pmt.cons(pmt.intern(self.outgoingPairName),pmt.to_pmt(incomingVal)))
      
    except Exception as e:
      print "[MetaToPair] Error with message conversion: %s" % str(e)
      print str(meta)    
    
  def stop(self):
    return True
