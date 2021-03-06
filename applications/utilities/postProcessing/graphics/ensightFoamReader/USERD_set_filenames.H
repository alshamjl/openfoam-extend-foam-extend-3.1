//======================================================================
//  Setting filenames
//======================================================================
int USERD_set_filenames
(
    char filename_1[],
    char filename_2[],
    char the_path[],
    int swapbytes
)
{
#ifdef ENSIGHTDEBUG
    Info << "Entering: USERD_set_filenames" << endl << flush;
#endif

    char tmp[100];

    label lRoot = strlen(the_path);
    label lCase = strlen(filename_1);

    bool cleared = false;

    while (!cleared)
    {
        lRoot = strlen(the_path);
        lCase = strlen(filename_1);

        // remove the last '/' from rootDir
        if (the_path[lRoot-1] == '/')
        {
            the_path[lRoot-1] = (char)NULL;
        }
        else
        {
            cleared = true;
        }
    }

    rootDir = the_path;

    // the path is pre-pended to filename_1
    // 1 is the 'Geometry' : 2 the 'Result' which is null here
    // since two_field is FALSE
    for (label i=0; i<lCase-lRoot;i++)
    {
        tmp[i] = filename_1[i+1+lRoot];
    }
    caseDir = tmp;

    if (!isDir(rootDir/caseDir))
    {
       Info<< rootDir/caseDir << " is not a valid directory."
           << endl;
       return Z_ERR;
    }

    // construct the global pointers to the database and mesh

    delete meshPtr;
    delete runTimePtr;

    runTimePtr = new Time
    (
        Time::controlDictName,
        rootDir,
        caseDir
    );

    Time& runTime = *runTimePtr;

    meshPtr = new fvMesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            runTime.timeName(),
            runTime
        )
    );

    // set the available number of time-steps
    timeDirs = Foam::Time::findTimes(rootDir/caseDir);

    Num_time_steps = timeDirs.size() - 1;

    nPatches = meshPtr->boundaryMesh().size();

    // set the number of fields and store their names
    // a valid field must exist for all time-steps
    runTime.setTime(timeDirs[timeDirs.size()-1], timeDirs.size()-1);
    IOobjectList objects(*meshPtr, runTime.timeName());

    fieldNames = objects.names();

    // because of the spray being a 'field' ...
    // get the availabe number of variables and
    // check for type (scalar/vector/tensor)

    label nVar = 0;
    wordList scalars = objects.names(scalarName);

    for (label n=0; n<fieldNames.size(); n++)
    {
        bool isitScalar = false;
        forAll(scalars,i)
        {
            if (fieldNames[n] == scalars[i])
            {
                isitScalar = true;
                var2field[nVar++] = n;
            }
        }
        isScalar[n] = isitScalar;
    }

    wordList vectors = objects.names(vectorName);

    for (label n=0; n<fieldNames.size(); n++)
    {
        bool isitVector = false;
        forAll(vectors,i)
        {
            if (fieldNames[n] == vectors[i])
            {
                isitVector = true;
                var2field[nVar++] = n;
            }
        }
        isVector[n] = isitVector;
    }

    wordList tensors = objects.names(tensorName);

    for (label n=0; n<fieldNames.size(); n++)
    {
        bool isitTensor = false;
        forAll(tensors,i)
        {
            if (fieldNames[n] == tensors[i])
            {
                isitTensor = true;
                var2field[nVar++] = n;
            }
        }
        isTensor[n] = isitTensor;
    }

    bool lagrangianNamesFound = false;
    label n = 0;
    while (!lagrangianNamesFound && n < Num_time_steps)
    {
        runTime.setTime(timeDirs[n+1], n+1);

        Cloud<passiveParticle> lagrangian(*meshPtr);

        n++;
        if (lagrangian.size())
        {
            lagrangianNamesFound = true;
        }
    }

    IOobject sprayHeader
    (
        "positions",
        runTime.timeName(),
        cloud::prefix,
        runTime,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
    );

    if (sprayHeader.headerOk())
    {
        Info << "[Found lagrangian]" << endl;

        delete sprayPtr;

        sprayPtr = new Cloud<passiveParticle>(*meshPtr);

        IOobjectList objects(*meshPtr, runTime.timeName(), cloud::prefix);

        lagrangianScalarNames =
            (const wordList&)objects.names(sprayScalarFieldName);
        lagrangianVectorNames =
            (const wordList&)objects.names(sprayVectorFieldName);

        isSpray[fieldNames.size()] = true;

        nSprayVariables += lagrangianScalarNames.size();
        nSprayVariables += lagrangianVectorNames.size();

        Num_unstructured_parts++;
    }

    Current_time_step = Num_time_steps;
    runTime.setTime(timeDirs[Current_time_step], Current_time_step);

    Num_variables = nVar + nSprayVariables;
    Numparts_available = Num_unstructured_parts + Num_structured_parts + nPatches;

#ifdef ENSIGHTDEBUG
    Info << "Leaving: USERD_set_filenames" << endl << flush;
#endif

    return Z_OK;
}

