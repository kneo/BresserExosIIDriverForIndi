/*
 * BresserExosIIGoToDriver.hpp
 * 
 * Copyright 2020 Kevin Krüger <kkevin@gmx.net>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#ifndef _BRESSEREXOSIIGOTODRIVER_H_INCLUDED_
#define _BRESSEREXOSIIGOTODRIVER_H_INCLUDED_

#include <cstdint>
#include <cmath>
#include <memory>
#include <string>

#include <libindi/indicom.h>
#include <libindi/inditelescope.h>
#include <libindi/indiguiderinterface.h>
#include <libindi/indilogger.h>

#include "IndiSerialWrapper.hpp"
#include "ExosIIMountControl.hpp"
#include "SerialDeviceControl/SerialCommand.hpp"

#include "Config.hpp"

namespace GoToDriver
{
	struct GuideState
	{
		SerialDeviceControl::SerialCommandID direction;
		uint32_t remaining_messages;
	};
	
	//Main wrapper class for the indi driver interface.
	//"Glues" together the independent functionallity with the driver interface from indi.
	class BresserExosIIDriver : public INDI::Telescope, public INDI::GuiderInterface
	{
		public:
			//default constructor.
			//sets the scope abilities, and default settings.
			BresserExosIIDriver();
			
			//destructor, not much going on here. Since most of the memory is statically allocated, there is not much to clean up.
			virtual ~BresserExosIIDriver();
			
			//initialize the properties of the scope.
			virtual bool initProperties();
			
			//update the properties of the scope visible in the EKOS dialogs for instance.
			virtual bool updateProperties();
			
			//Connect to the scope, and ready everything for serial data exchange.
			virtual bool Connect();
			
			//Start the serial receiver thread, so the mount can report its pointing coordinates.
			virtual bool Handshake();

			//Disconnect from the mount, and disable serial transmission.
			virtual bool Disconnect();
			
			//Return the name of the device, displayed in the e.g. EKOS dialogs
			virtual const char* getDefaultName();
			
			//Periodically polled function to update the state of the driver, and synchronize it with the mount.
			virtual bool ReadScopeStatus();
			
			//update properties from the application -> number
			virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n);
			
			//update properties from the application -> text
			virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n);
			
			//Park the telescope. This will slew the telescope to the parking position == home position.
			virtual bool Park();
			
			//Set the state of the driver to unpark allowing the scope to be manipulated again.
			virtual bool UnPark();

			//Sync the astro software and mount coordinates.
			virtual bool Sync(double ra, double dec);
			
			//Go to the coordinates in the sky, This automatically tracks the selected coordinates. 
			virtual bool Goto(double ra, double dec);
			
			//Abort any motion of the telescope. This is state indipendent, and always possible when connected.
			virtual bool Abort();
			
			//Set the tracking state of the scope, it either goes to the current coordinates or stops the scope motion.
			virtual bool SetTrackingEnabled(bool enabled);

			//update the time of the scope.
			virtual bool updateTime(ln_date *utc, double utc_offset);
			
			//update the location of the scope.
			virtual bool updateLocation(double latitude, double longitude, double elevation);
			
			//commance motion in north or south direction.
			virtual bool MoveNS(INDI_DIR_NS dir, TelescopeMotionCommand command) override;
			
			//commance motion in east or west direction.
			virtual bool MoveWE(INDI_DIR_WE dir, TelescopeMotionCommand command) override;
			
			// Guide
			virtual IPState GuideNorth(uint32_t ms) override;
			virtual IPState GuideSouth(uint32_t ms) override;
			virtual IPState GuideEast(uint32_t ms) override;
			virtual IPState GuideWest(uint32_t ms) override;

			// Pulse Guide timeout callbacks.
			static void guideTimeoutHelperN(void *p);
			static void guideTimeoutHelperS(void *p);
			static void guideTimeoutHelperE(void *p);
			static void guideTimeoutHelperW(void *p);
			
			void guideTimeout(SerialDeviceControl::SerialCommandID direction);

			//GUIDE timer id variables.
			int GuideNSTID;
			int GuideWETID;
			
			
			
		private:
			IndiSerialWrapper mInterfaceWrapper;
			
			TelescopeMountControl::ExosIIMountControl<IndiSerialWrapper> mMountControl;
			
			unsigned int DBG_SCOPE;
			
			GuideState mGuideStateNS;
			GuideState mGuideStateEW;
	};
}

#endif
