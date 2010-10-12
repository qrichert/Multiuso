/*

Copyright © 2009-2010 Quentin RICHERT

Multiuso is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Multiuso is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Multiuso.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef HEADER_COPYCUT
#define HEADER_COPYCUT

#include "../CurrentIncludes.h"

enum CopyCutAction
{
	COPY, CUT, NONE
};

class CopyCutObject
{
	public:
		CopyCutObject()
		{
			m_currentAction = NONE;
			m_fileLink = "";
			m_fileType = "";
		}

		void setCurrentAction(CopyCutAction currentAction)
		{
			m_currentAction = currentAction;
		}

		CopyCutAction currentAction()
		{
			return m_currentAction;
		}

		void setFileLink(QString fileLink)
		{
			m_fileLink = fileLink;
		}

		QString fileLink()
		{
			return m_fileLink;
		}
		
		void setFileType(QString fileType)
		{
			m_fileType = fileType;
		}

		QString fileType()
		{
			return m_fileType;
		}

	private:
		CopyCutAction m_currentAction;
		QString m_fileLink;
		QString m_fileType;
};

#endif
