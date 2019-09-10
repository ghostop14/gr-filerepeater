#!/usr/bin/env python

from gnuradio import gr
import pmt

class StateMessageAnd(gr.sync_block):
  def __init__(self):
    gr.sync_block.__init__(self, name = "State Message AND", in_sig = None, out_sig = None)
    
    self.message_port_register_in(pmt.intern("state1"))
    self.message_port_register_in(pmt.intern("state2"))
    self.message_port_register_out(pmt.intern("state"))
    self.set_msg_handler(pmt.intern("state1"), self.state1Handler)   
    self.set_msg_handler(pmt.intern("state2"), self.state2Handler)
    
    self.state1 = False
    self.state2 = False
    
    self.curState = False   

  def state1Handler(self, pdu):
    try:    
      newState = pmt.to_python(pmt.cdr(pdu))
      # print "Received message 1: %d " % newState
      if newState == 1:
        self.state1 = True
      else:
        self.state1 = False
      
      if (self.state1 and self.state2):
        self.sendState(True)
        self.curState = True
      else:
        if (self.curState):
          self.sendState(False)
          self.curState = False
    except Exception as e:
      print("Error with state1 message: %s" % str(e))
      print(str(meta))    
            
  def state2Handler(self, pdu):
    try:    
      newState = pmt.to_python(pmt.cdr(pdu))
      
      # print "Received message 2: %d " % newState
      if newState == 1:
        self.state2 = True
      else:
        self.state2 = False
      
      if (self.state1 and self.state2):
        self.sendState(True)
        self.curState = True
      else:
        if (self.curState):
          self.sendState(False)
          self.curState = False
    except Exception as e:
      print("Error with state2 message: %s" % str(e))
      print(str(meta))    
      
  def sendState(self,state):
    if (state):    
      newState = 1
    else:
      newState = 0
      
    self.message_port_pub(pmt.intern("state"),pmt.cons( pmt.intern("state"), pmt.from_long(newState) ))
