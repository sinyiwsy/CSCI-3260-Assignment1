#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 modelTransformMatrix;
uniform mat4 modelScalingMatrix;
uniform mat4 modelRotationMatrix;
uniform mat4 projectionMatrix;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 new_position = modelTransformMatrix * modelScalingMatrix * modelRotationMatrix * v;
	vec4 projectedPosition = projectionMatrix * new_position;
	gl_Position = projectedPosition;	
	theColor = vertexColor;
}