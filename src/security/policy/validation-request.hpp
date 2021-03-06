/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Yingdi Yu <yingdi@cs.ucla.edu>
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_VALIDATION_REQUEST_HPP
#define NDN_VALIDATION_REQUEST_HPP

#include <ndn-cpp/security/key-chain.hpp>

namespace ndn {

class ValidationRequest {
public:
  ValidationRequest
    (const ptr_lib::shared_ptr<Interest> &interest, const OnVerified& onVerified, const OnVerifyFailed& onVerifyFailed,
     const int& retry, const int& stepCount)
  : interest_(interest), onVerified_(onVerified), onVerifyFailed_(onVerifyFailed), retry_(retry), stepCount_(stepCount)
  {
  }
    
  virtual
  ~ValidationRequest() {}

  ptr_lib::shared_ptr<Interest> interest_; // An interest packet to fetch the requested data.
  OnVerified onVerified_;                  // A callback function if the requested certificate has been validated.
  OnVerifyFailed onVerifyFailed_;          // A callback function if the requested certificate cannot be validated.
  int retry_;                              // The number of retrials when there is an interest timeout.
  int stepCount_;
};

}

#endif
