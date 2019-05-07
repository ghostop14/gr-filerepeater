#!/usr/bin/env python
# 

from gnuradio import gr
import pmt

class StateToBool(gr.sync_block):
  def __init__(self):
    gr.sync_block.__init__(self, name = "State Message To Boolean", in_sig = None, out_sig = None)
    
    self.message_port_register_in(pmt.intern("state"))
    self.message_port_register_out(pmt.intern("bool"))
    self.set_msg_handler(pmt.intern("state"), self.stateHandler)   

  def stateHandler(self, pdu):
    meta = pmt.to_python(pmt.car(pdu))
    
    try:    
      newState = int(meta['state'])
      # print "Received message 1: %d " % newState
      if newState == 1:
        boolState = True
      else:
        boolState = False
      
      self.message_port_pub(pmt.intern("bool"),pmt.to_pmt(boolState))
      
    except Exception as e:
      print "[State to Bool] Error with state message: %s" % str(e)
      print str(meta)    
