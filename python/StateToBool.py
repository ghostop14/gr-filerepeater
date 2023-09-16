#!/usr/bin/env python
# 

from gnuradio import gr
import pmt

class StateToBool(gr.sync_block):
  def __init__(self,  invert = False):
    gr.sync_block.__init__(self, name = "State Message To Boolean", in_sig = None, out_sig = None)
    
    self.message_port_register_in(pmt.intern("state"))
    self.message_port_register_out(pmt.intern("bool"))
    self.set_msg_handler(pmt.intern("state"), self.stateHandler)   
    self.invert = invert
    
  def stateHandler(self, pdu):
    try:    
      newState = pmt.to_python(pmt.cdr(pdu))
      # print "Received message 1: %d " % newState
      if newState == 1:
        boolState = True
      else:
        boolState = False
      
      if self.invert:
          boolState = not boolState
          
      self.message_port_pub(pmt.intern("bool"),pmt.to_pmt(boolState))
      
    except Exception as e:
      print("[State to Bool] Error with state message: %s" % str(e))
      print(str(newState))    
