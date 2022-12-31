# Multiclass Classification

### Description

A simple classification problem of data with 5 classes using a neural network with only one layer and sigmoid activation. Network performance is shown below for 4 and 10 epochs.

##### Sample Output after 4 Epochs:

<img alt="Classification-35-Points-4-Epochs" src="/Samples/NN-Sample-01/Classification-35-Points-4-Epochs.jpg" width="250px"></img>

##### Sample Output after 10 Epochs:

<img alt="Classification-35-Points-10-Epochs" src="/Samples/NN-Sample-01/Classification-35-Points-10-Epochs.jpg" width="250px"></img>

- The data points and their labels are loaded from [`Points-35.dbs`](/Samples/NN-Sample-01/Data/Points-35.dbs)
- Neural Network topology, optimizer, number of layers and layer parameters are saved in [`NN-Model-Points.snn`](/Samples/NN-Sample-01/Data/NN-Model-Points.snn)

## Setup

Create an **Empty Project** using Visual Studio and copy all the files from NN-Sample-01 to the project.

And follow [Project Setup](/Setup/) to run the program.

### Required Header Files

- SepMath.h
- NeuralNetwork.h
- Layer.h
- Dir.h
- Util.h
