/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     |
    \\  /    A nd           | For copyright notice see file Copyright
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
    mixingPlaneCheckFunctionObject

Description
    FunctionObject checking the fluxes on mixingPlane interfaces.

Author
    Martin Beaudoin, Hydro-Quebec, 2009.  All rights reserved

SourceFiles
    mixingPlaneCheckFunctionObject.C

\*---------------------------------------------------------------------------*/

#ifndef mixingPlaneCheckFunctionObject_H
#define mixingPlaneCheckFunctionObject_H

#include "functionObject.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class mixingPlaneCheckFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class mixingPlaneCheckFunctionObject
:
    public functionObject
{
    // Private data

        //- Reference to main object registry
        const Time& time_;

        //- Region name
        word regionName_;

        //- Name of flux field
        word phiName_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        mixingPlaneCheckFunctionObject(const mixingPlaneCheckFunctionObject&);

        //- Disallow default bitwise assignment
        void operator=(const mixingPlaneCheckFunctionObject&);


public:

    //- Runtime type information
    TypeName("mixingPlaneCheck");


    // Constructors

        //- Construct from components
        mixingPlaneCheckFunctionObject
        (
            const word& name,
            const Time&,
            const dictionary&
        );


    // Member Functions

        //- start is called at the start of the time-loop
        virtual bool start();

        //- execute is called at each ++ or += of the time-loop
        virtual bool execute();

        //- Read and set the function object if its data has changed
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
