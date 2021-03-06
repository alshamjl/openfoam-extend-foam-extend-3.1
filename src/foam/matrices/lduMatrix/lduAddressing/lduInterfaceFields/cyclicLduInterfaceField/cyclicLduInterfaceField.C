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

\*---------------------------------------------------------------------------*/

#include "cyclicLduInterfaceField.H"
#include "diagTensorField.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(cyclicLduInterfaceField, 0);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::cyclicLduInterfaceField::~cyclicLduInterfaceField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::cyclicLduInterfaceField::transformCoupleField
(
    scalarField& pnf,
    const direction cmpt
) const
{
    if (doTransform())
    {
        label sizeby2 = pnf.size()/2;

        scalar forwardScale =
            pow(diag(forwardT()[0]).component(cmpt), rank());

        scalar reverseScale =
            pow(diag(reverseT()[0]).component(cmpt), rank());

        for (label facei=0; facei<sizeby2; facei++)
        {
            pnf[facei] *= forwardScale;
            pnf[facei + sizeby2] *= reverseScale;
        }
    }
}


// ************************************************************************* //
