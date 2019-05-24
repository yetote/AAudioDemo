package com.example.aaudiodemo.objects;

import android.opengl.GLES20;
import android.opengl.GLES30;

import com.example.aaudiodemo.data.VertexArray;
import com.example.aaudiodemo.programs.BezierProgram;

import static android.opengl.GLES20.glLineWidth;

/**
 * @author ether QQ:503779938
 * @name AAudioDemo
 * @class name：com.example.aaudiodemo.objects
 * @class describe
 * @time 2019/5/24 17:10
 * @change
 * @chang time
 * @class describe
 */
public class BezierObject {
    public static final int POSITION_COMPONENT_COUNT = 2;
    public static final int STRIDE = POSITION_COMPONENT_COUNT * 4;
    private final float[] vertexData = new float[]{
            //    @formatter:off
            -1.0f, -0.3f,
            -0.7f,  0.7f,
             0.2f,  0.2f,
             0.5f,  0.6f,
             0.7f,  0.5f,
             1.0f, -0.3f,
            //    @formatter:on
    };
    private VertexArray vertexArray;

    public BezierObject(VertexArray vertexArray) {
        this.vertexArray = vertexArray;
    }
    public void bindData(BezierProgram program) {
        vertexArray.setVertexAttributePointer(0, program.getAttrPositionLocation(), POSITION_COMPONENT_COUNT, STRIDE);
        vertexArray.setVertexAttributePointer(POSITION_COMPONENT_COUNT, program.getAttrTexCoordLocation(), TEXTURE_COMPONENT_COUNT, STRIDE);
    }
    public void draw() {
        GLES30.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6);
        glLineWidth();
    }

}
