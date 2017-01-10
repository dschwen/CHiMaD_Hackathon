#!/usr/bin/env python
import chigger
import sys
import vtk

reader = chigger.exodus.ExodusReader(sys.argv[1], timestep = int(sys.argv[2]))

camera = vtk.vtkCamera()
camera.SetViewUp(0.0000, 1.0000, 0.0000)
camera.SetPosition(100.0000, 100.0000, 375.0)
camera.SetFocalPoint(100.0000, 100.0000, 0.0000)

# model = chigger.exodus.ExodusResult(reader, variable=sys.argv[3], cmap='viridis', background=[1,1,1], camera=camera)
model = chigger.exodus.ExodusResult(reader, variable=sys.argv[3], camera=camera)
window = chigger.RenderWindow(model, size=[800,800], test=True)
window.write('out.png')

#print chigger.utils.printcamera(model.getVTKRenderer().GetActiveCamera())
window.start()
