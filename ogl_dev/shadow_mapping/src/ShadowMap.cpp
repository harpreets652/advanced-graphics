/*
        Copyright 2011 Etay Meiri
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ShadowMap.h"

ShadowMap::ShadowMap() {
    fboHandler = 0;
    shadowMapHandler = 0;
}

ShadowMap::~ShadowMap() {
    if (fboHandler != 0) {
        glDeleteFramebuffers(1, &fboHandler);
    }

    if (shadowMapHandler != 0) {
        glDeleteTextures(1, &shadowMapHandler);
    }
}

bool ShadowMap::init(int WindowWidth, int WindowHeight) {
    // Create the FBO
    glGenFramebuffers(1, &fboHandler);

    // Create the depth buffer
    glGenTextures(1, &shadowMapHandler);
    glBindTexture(GL_TEXTURE_2D, shadowMapHandler);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 WindowWidth,
                 WindowHeight,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandler);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapHandler, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error initializing Frame buffer: " << status << std::endl;
        return false;
    }

    return true;
}

void ShadowMap::bindForWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboHandler);
}


void ShadowMap::bindForReading(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMapHandler);
}