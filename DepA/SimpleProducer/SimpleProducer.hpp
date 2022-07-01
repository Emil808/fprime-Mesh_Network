// ======================================================================
// \title  SimpleProducer.hpp
// \author emil
// \brief  hpp file for SimpleProducer component implementation class
// ======================================================================

#ifndef SimpleProducer_HPP
#define SimpleProducer_HPP

#include "DepA/SimpleProducer/SimpleProducerComponentAc.hpp"

namespace DepA {
  
  class SimpleProducer :
    public SimpleProducerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SimpleProducer
      //!
      SimpleProducer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object SimpleProducer
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object SimpleProducer
      //!
      ~SimpleProducer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for SET_NUM command handler
      //! Produce a number and send it to Consumer
      void SET_NUM_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          F32 val1 /*!< 
          Number to send to Consumer
          */
      );


    };
} // end namespace DepA

#endif
