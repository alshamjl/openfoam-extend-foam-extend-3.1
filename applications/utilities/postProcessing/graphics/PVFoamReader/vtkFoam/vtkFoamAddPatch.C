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

Description

\*---------------------------------------------------------------------------*/

#include "vtkFoam.H"
#include "polyPatch.H"

#include "vtkUnstructuredGrid.h"
#include "vtkCellArray.h"

#include "vtkFoamInsertNextPoint.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::vtkFoam::addPatch
(
    const polyPatch& p,
    vtkUnstructuredGrid *vtkPatch
)
{
    if (debug)
    {
        Info<< "Adding patch " << p.name() << endl;
    }

    SetName(vtkPatch, p.name().c_str());

    if (debug)
    {
        Info<< "converting points" << endl;
    }

    const Foam::pointField& points = p.localPoints();

    // Convert Foam mesh vertices to VTK
    vtkPoints *vtkpoints = vtkPoints::New();
    vtkpoints->Allocate(points.size());

    forAll(points, i)
    {
        vtkFoamInsertNextPoint(vtkpoints, points[i]);
    }

    if (debug)
    {
        Info<< "converting faces" << endl;
    }

    const faceList& faces = p.localFaces();

    vtkPatch->Allocate(faces.size());

    forAll(faces, facei)
    {
        const face& f = faces[facei];

        if (f.size() == 3)
        {
            vtkPatch->InsertNextCell
            (
                VTK_TRIANGLE,
                3,
                const_cast<int*>(f.begin())
            );
        }
        else if (f.size() == 4)
        {
            vtkPatch->InsertNextCell
            (
                VTK_QUAD,
                4,
                const_cast<int*>(f.begin())
            );
        }
        else
        {
            vtkPatch->InsertNextCell
            (
                VTK_POLYGON,
                f.size(),
                const_cast<int*>(f.begin())
            );
        }
    }

    vtkPatch->SetPoints(vtkpoints);
    vtkpoints->Delete();
}


// ************************************************************************* //
