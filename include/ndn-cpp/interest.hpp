/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_INTEREST_HPP
#define NDN_INTEREST_HPP

#include "name.hpp"
#include "publisher-public-key-digest.hpp"
#include "c/interest-types.h"
#include "encoding/wire-format.hpp"

struct ndn_ExcludeEntry;
struct ndn_Exclude;
struct ndn_Interest;

namespace ndn {
  
/**
 * An ExcludeEntry holds an ndn_ExcludeType, and if it is a COMPONENT, it holds the component value.
 */
class ExcludeEntry {
public:
  /**
   * Create an ExcludeEntry of type ndn_Exclude_ANY
   */
  ExcludeEntry()
  : type_(ndn_Exclude_ANY)
  {    
  }
  
  /**
   * Create an ExcludeEntry of type ndn_Exclude_COMPONENT.
   */
  ExcludeEntry(uint8_t *component, size_t componentLen) 
  : type_(ndn_Exclude_COMPONENT), component_(component, componentLen)
  {
  }
  
  /**
   * Create an ExcludeEntry of type ndn_Exclude_COMPONENT.
   */
  ExcludeEntry(const Blob& component) 
  : type_(ndn_Exclude_COMPONENT), component_(component)
  {
  }
  
  /**
   * Set the type in the excludeEntryStruct and to point to this component, without copying any memory.
   * WARNING: The resulting pointer in excludeEntryStruct is invalid after a further use of this object which could reallocate memory.
   * @param excludeEntryStruct the C ndn_NameComponent struct to receive the pointer
   */
  void 
  get(struct ndn_ExcludeEntry& excludeEntryStruct) const;
  
  ndn_ExcludeType getType() const { return type_; }
  
  const Name::Component& getComponent() const { return component_; }
  
private:
  ndn_ExcludeType type_;
  Name::Component component_; /**< only used if type_ is ndn_Exclude_COMPONENT */
}; 
  
/**
 * An Exclude holds a vector of ExcludeEntry.
 */
class Exclude {
public:
  /**
   * Create a new Exclude with no entries.
   */
  Exclude() {
  }
  
  size_t 
  getEntryCount() const {
    return entries_.size();
  }
  
  const ExcludeEntry& 
  getEntry(size_t i) const { return entries_[i]; }
  
  /**
   * Set the excludeStruct to point to the entries in this Exclude, without copying any memory.
   * WARNING: The resulting pointers in excludeStruct are invalid after a further use of this object which could reallocate memory.
   * @param excludeStruct a C ndn_Exclude struct where the entries array is already allocated
   */
  void 
  get(struct ndn_Exclude& excludeStruct) const;
  
  /**
   * Clear this Exclude, and set the entries by copying from the ndn_Exclude struct.
   * @param excludeStruct a C ndn_Exclude struct
   */
  void 
  set(const struct ndn_Exclude& excludeStruct);

  /**
   * Append a new entry of type ndn_Exclude_ANY.
   * @return This Exclude so that you can chain calls to append.
   */
  Exclude& 
  appendAny()
  {    
    entries_.push_back(ExcludeEntry());
    return *this;
  }
  
  /**
   * Append a new entry of type ndn_Exclude_COMPONENT, copying from component of length componentLength.
   * @param component A pointer to the component byte array.
   * @param componentLength The length of component.
   * @return This Exclude so that you can chain calls to append.
   */
  Exclude& 
  appendComponent(uint8_t *component, size_t componentLength) 
  {
    entries_.push_back(ExcludeEntry(component, componentLength));
    return *this;
  }

  /**
   * Append a new entry of type ndn_Exclude_COMPONENT, taking another pointer to the Blob value.
   * @param component A blob with a pointer to an immutable array.  The pointer is copied.
   * @return This Exclude so that you can chain calls to append.
   */
  Exclude& 
  appendComponent(const Blob &component) 
  {
    entries_.push_back(ExcludeEntry(component));
    return *this;
  }

  /**
   * @deprecated Use appendAny.
   */
  Exclude& 
  addAny() { return appendAny(); }

  /**
   * @deprecated Use appendComponent.
   */
  Exclude& 
  addComponent(uint8_t *component, size_t componentLength) { return appendComponent(component, componentLength); }
  
  /**
   * Clear all the entries.
   */
  void 
  clear() {
    entries_.clear();
  }
  
  /**
   * Encode this Exclude with elements separated by "," and ndn_Exclude_ANY shown as "*".
   * @return the URI string
   */
  std::string toUri() const;
  
private:
  std::vector<ExcludeEntry> entries_;
};

/**
 * An Interest holds a Name and other fields for an interest.
 */
class Interest {
public:    
  Interest(const Name& name, int minSuffixComponents, int maxSuffixComponents, 
    const PublisherPublicKeyDigest& publisherPublicKeyDigest, const Exclude& exclude, int childSelector, int answerOriginKind, 
    int scope, Milliseconds interestLifetimeMilliseconds, const std::vector<uint8_t>& nonce) 
  : name_(name), minSuffixComponents_(minSuffixComponents), maxSuffixComponents_(maxSuffixComponents),
  publisherPublicKeyDigest_(publisherPublicKeyDigest), exclude_(exclude), childSelector_(childSelector), 
  answerOriginKind_(answerOriginKind), scope_(scope), interestLifetimeMilliseconds_(interestLifetimeMilliseconds),
  nonce_(nonce)
  {
  }

  Interest(const Name& name, int minSuffixComponents, int maxSuffixComponents, 
    const PublisherPublicKeyDigest& publisherPublicKeyDigest, const Exclude& exclude, int childSelector, int answerOriginKind, 
    int scope, Milliseconds interestLifetimeMilliseconds) 
  : name_(name), minSuffixComponents_(minSuffixComponents), maxSuffixComponents_(maxSuffixComponents),
  publisherPublicKeyDigest_(publisherPublicKeyDigest), exclude_(exclude), childSelector_(childSelector), 
  answerOriginKind_(answerOriginKind), scope_(scope), interestLifetimeMilliseconds_(interestLifetimeMilliseconds)
  {
  }

  Interest(const Name& name, Milliseconds interestLifetimeMilliseconds) 
  : name_(name)
  {
    construct();
    interestLifetimeMilliseconds_ = interestLifetimeMilliseconds;
  }

  Interest(const Name& name) 
  : name_(name)
  {
    construct();
  }

  Interest() 
  {
    construct();
  }
  
  Blob 
  wireEncode(WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) const 
  {
    return wireFormat.encodeInterest(*this);
  }
  
  void 
  wireDecode(const uint8_t *input, size_t inputLength, WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) 
  {
    wireFormat.decodeInterest(*this, input, inputLength);
  }
  
  void 
  wireDecode(const std::vector<uint8_t>& input, WireFormat& wireFormat = *WireFormat::getDefaultWireFormat()) 
  {
    wireDecode(&input[0], input.size(), wireFormat);
  }
  
  /**
   * Set the interestStruct to point to the components in this interest, without copying any memory.
   * WARNING: The resulting pointers in interestStruct are invalid after a further use of this object which could reallocate memory.
   * @param interestStruct a C ndn_Interest struct where the name components array is already allocated.
   */
  void 
  get(struct ndn_Interest& interestStruct) const;

  Name& 
  getName() { return name_; }
  
  const Name& 
  getName() const { return name_; }
  
  int 
  getMinSuffixComponents() const { return minSuffixComponents_; }
  
  int 
  getMaxSuffixComponents() const { return maxSuffixComponents_; }
  
  PublisherPublicKeyDigest& 
  getPublisherPublicKeyDigest() { return publisherPublicKeyDigest_; }
  
  const PublisherPublicKeyDigest& 
  getPublisherPublicKeyDigest() const { return publisherPublicKeyDigest_; }

  Exclude& 
  getExclude() { return exclude_; }
  
  const Exclude& 
  getExclude() const { return exclude_; }
  
  int 
  getChildSelector() const { return childSelector_; }

  int 
  getAnswerOriginKind() const { return answerOriginKind_; }

  int 
  getScope() const { return scope_; }

  Milliseconds 
  getInterestLifetimeMilliseconds() const { return interestLifetimeMilliseconds_; }

  const Blob& 
  getNonce() const { return nonce_; }
  
  /**
   * Clear this interest, and set the values by copying from the interest struct.
   * @param interestStruct a C ndn_Interest struct
   */
  void 
  set(const struct ndn_Interest& interestStruct);
  
  void
  setName(const Name& name) { name_ = name; }
  
  void 
  setMinSuffixComponents(int value) { minSuffixComponents_ = value; }
  
  void 
  setMaxSuffixComponents(int value) { maxSuffixComponents_ = value; }
  
  void 
  setChildSelector(int value) { childSelector_ = value; }

  void 
  setAnswerOriginKind(int value) { answerOriginKind_ = value; }

  void 
  setScope(int value) { scope_ = value; }

  void 
  setInterestLifetimeMilliseconds(Milliseconds value) { interestLifetimeMilliseconds_ = value; }

  void 
  setNonce(const std::vector<uint8_t>& value) { nonce_ = value; }
  
private:
  void 
  construct() 
  {
    minSuffixComponents_ = -1;
    maxSuffixComponents_ = -1;  
    childSelector_ = -1;
    answerOriginKind_ = -1;
    scope_ = -1;
    interestLifetimeMilliseconds_ = -1.0;
  }
  
  Name name_;
  int minSuffixComponents_;
  int maxSuffixComponents_;  
  PublisherPublicKeyDigest publisherPublicKeyDigest_;
  Exclude exclude_;
  int childSelector_;
  int answerOriginKind_;
  int scope_;
  Milliseconds interestLifetimeMilliseconds_;
  Blob nonce_;
};
  
}

#endif
