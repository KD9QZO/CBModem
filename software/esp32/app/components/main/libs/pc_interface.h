/**
 * \file pc_interface.h
 * \brief Describes the interface to the PC host
 */

#ifndef PC_INTERFACE_H_
#define PC_INTERFACE_H_

#include <stdint.h>


#if (!defined(UART_BUADRATE) || defined(__DOXYGEN__))
/**
 * \brief The baudrate at which the PC host interface will operate at
 *
 * \note If \c UART_BAUDRATE is not defined by the build system, the default baudrate of \b 115200 will be used.
 */
#define UART_BAUDRATE (115200)
#endif


/**
 * \brief The PC Packet Interface
 *
 * The interface uses binary packets, starting with \ref startByte
 *
 * The format of a packet is:
 * + \b StartByte
 * + \b PacketType
 * + \b PacketLength (the length of the \b PacketData that follows)
 * + \b PacketData (a maximum of \b 256 bytes)
 * + \b PacketChecksum
 *
 * This is designed to work with special applications, including SDR software.
 *
 * \version 1.3
 */
class pc_packet_interface {
public:
	/*! \brief The constant value, used at the start of every packet */
	static const uint8_t startByte = 0x7C;

	/*! \brief The types of packets that can originate from the CBModem device to the PC host */
	enum packetType_fromdev {
		PC_PI_PTD_START,			/*!< Startup initialization ack(after reset), no data */
		PC_PI_PTD_ACK,				/*!< Acknowledge, data: 1 byte = 1 or 0, depending on last packet execution result */
		PC_PI_PTD_SDR_RX_DATA,		/*!< Received data in SDR mode, data: N complex vals(2 int16 - i & q) */
		PC_PI_PTD_N_TRANSMIT_COMPL,	/*!< Data transmitting in normal mode completed, no data */
		PC_PI_PTD_N_RX_DATA,		/*!< Data receiving in normal mode completed, data: 1 byte - info(6 low bits-signal quality, 2 high bits-status(0-SW read, 1-type+len read, 2-data read, 3-ack read)) + N received bytes */
		PC_PI_PTD_PARAM_DATA,		/*!< Requested parameter value, data: N bytes - data */
	};

	/*! \brief The types of packets that can originate from the PC host to the CBModem device */
	enum packetType_frompc {
		PC_PI_PTP_CHANGE_MODE,		/*!< Change device mode, data: 1 byte - new mode */
		PC_PI_PTP_SET_FR,			/*!< Change center frequency, data: 1 float - new freq */
		PC_PI_PTP_RX_SET_IN_SRC,	/*!< Change RX input channels pair, data: 1 byte = 1 or 0, depending on selected channels pair */
		PC_PI_PTP_SET_SPD,			/*!< Change RX/TX sample rate in SDR mode, bit rate in normal mode(+frequencies), data: in SDR mode 1 float - new sr, in normal bfsk mode 2 floats - new bit rate, frequency difference, in mfsk mode 3 floats - new bit rate, frequency difference, frequency count, in msk mode 1 float - new bit rate */
		PC_PI_PTP_RX_START,			/*!< Start receiving in current selected mode, no data */
		PC_PI_PTP_RX_STOP,			/*!< Stop receiving in current selected mode, no data */
		PC_PI_PTP_TX_START,			/*!< Start transmitting in current selected mode, no data */
		PC_PI_PTP_TX_STOP,			/*!< Stop transmitting in current selected mode, no data */
		PC_PI_PTP_TX_DATA,			/*!< Put data to tx buffer, data: in SDR mode N complex vals(2 int16 - i & q), in normal mode N bytes of packet data */
		PC_PI_PTP_TX_CARRIER,		/*!< Start transmitting max amplitude carrier wave, no data */
		PC_PI_PTP_PARAM_READ,		/*!< Request reading parameter, data: 1 byte-name length(1-15), N bytes - name */
		PC_PI_PTP_PARAM_WRITE,		/*!< Write parameter to device, data: 1 byte-name length(1-15), N bytes - name, 1 byte-value length, K bytes - value */
		PC_PI_PTP_PARAM_STORE,		/*!< Store written params to flash, no data */
	};

	/*! \brief The modes that the CBModem device can operate in */
	enum modes {
		PC_PI_MODE_UNINITED,		/*!< The mode is \e uninitialized */
		PC_PI_MODE_NORMAL_BFSK,		/*!< Normal \b BFSK (binary frequency shift keying) mode */
		PC_PI_MODE_NORMAL_MFSK,		/*!< Normal \b MFSK (minimum frequency shift keying) mode */
		PC_PI_MODE_NORMAL_MSK,		/*!< Normal \b MSK (minimum shift keying) mode */
		PC_PI_MODE_SDR,				/*!< \b SDR (software defined radio) mode */
	};

	/**
	 * \brief The pc_packet class
	 */
	class pc_packet {
	public:
		/*! \brief The type of packet */
		uint8_t type;

		/*! \brief The length of the packet */
		uint8_t len;

		/**
		 * \brief The packet data
		 *
		 * \note The maximum \ref len of a packet is \b 256 bytes!
		 */
		uint8_t data[256];

		/*! \brief The checksum of the packet */
		uint8_t checksum;
	};

/**
 * \brief The maximum size of a packet between the PC host and the CBModem device
 *
 * The format of a packet is:
 * + \b StartByte
 * + \b PacketType
 * + \b PacketLength (the length of the \b PacketData that follows)
 * + \b PacketData (a maximum of \b 256 bytes)
 * + \b PacketChecksum
 */
#define MAX_PC_P_SIZE (1 + 1 + 1 + 256 + 1)
};


#endif	/* !PC_INTERFACE_H_ */
