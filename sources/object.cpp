#include "object.h"

extern "C" {
#include <libmesh5.h>
}
#define FLOAT_MAX 1.e20
/*
  Usually it is scale * rotation * translation. 
  However, if you want to rotate an object around a certain point,
  then it is scale * point_translation * rotation * object_translation.
*/

Object::Object(Scene* scene){
  parentScene = scene;
  createGeometry();
  computeBoundingBox();
  createAndBindBuffers();
}
Object::Object(Scene* scene, char * mesh_path){
  parentScene = scene;
  createGeometry(mesh_path);
  computeBoundingBox();
  scaleAndTranslate();
  createAndBindBuffers();
  MODEL = glm::mat4(1);
}

void Object::computeBoundingBox(){
  glm::vec3 bbmin = glm::vec3(FLOAT_MAX);
  glm::vec3 bbmax = glm::vec3(-FLOAT_MAX);
  for (int i=0; i<vertices.size(); i+=3) {
    for(int j = 0 ; j < 3 ; j++){
      if ( vertices[i+j] < bbmin[j] )
	bbmin[j] = vertices[i+j];
      if ( vertices[i+j] > bbmax[j] )
	bbmax[j] = vertices[i+j];
    }
  }
  translation = -0.5f * (bbmin + bbmax);
  glm::vec3 size    = bbmax - bbmin;
  float maxDim      = std::max( std::max(size.x, size.y) , size.z );
  scale     = 0.5f / maxDim;
}
void Object::scaleAndTranslate(){
  for(int i = 0 ; i < vertices.size() ; i+=3){
    for(int j = 0 ; j < 3 ; j++){
      vertices[i+j] += translation[j];
      vertices[i+j] *= scale;
    }
  }
}

void Object::createGeometry(){
  vertices = std::vector<float>{
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };
  for(int i = 0 ; i < vertices.size() ; i++)
    vertices[i] = 0.5f * vertices[i];
  nbVertices = vertices.size()/3;
  indices = std::vector<int>{
    0, 1, 2,
    2, 3, 0,
    3, 2, 6,
    6, 7, 3,
    7, 6, 5,
    5, 4, 7,
    4, 5, 1,
    1, 0, 4,
    4, 0, 3,
    3, 7, 4,
    1, 5, 6,
    6, 2, 1
  };
  colors = std::vector<float>{
    0.0f, 0.0f, 0.0f,   
    0.0f, 0.0f, 1.0f,   
    0.0f, 1.0f, 0.0f,   
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   
    1.0f, 0.0f, 1.0f,   
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f
  };
  normals = std::vector<float>{
    0, 0, 1,   
    0, 0, 1,   
    0, 1, 0,   
    0, 1, 1,
    1, 0, 0,   
    1, 0, 1,   
    1, 1, 0,
    1, 1, 1
  };
}

void Object::createGeometry(char * mesh_path){
  std::string meshFile    = std::string(mesh_path);

  //Initialisation
  int                     np,nt,nn,dim,ver, nNAtV;
  Point                   *ppt;
  Tria                    *pt;
  double                  *n,dd;
  float                   fp1,fp2,fp3;
  int                     k,inm;
  std::vector<Point>           point;
  std::vector<Tria>            tria;
  std::vector<Normal>          normal;
  std::vector<NormalAtVertex>  NormalAtVertices;

  //Lecture du .mesh
  inm = GmfOpenMesh(mesh_path,GmfRead,&ver,&dim);
  if ( !inm ){
    std::cout << "Unable to open mesh file " << mesh_path << std::endl;
    exit(-1);
  }

  np    = GmfStatKwd(inm, GmfVertices);
  nt    = GmfStatKwd(inm, GmfTriangles);
  nn    = GmfStatKwd(inm, GmfNormals);
  nNAtV = GmfStatKwd(inm, GmfNormalAtVertices);
  if ( !np ){
    std::cout << "No points found in mesh file" << mesh_path << std::endl;
    exit(-1);
  }
  if( !nt ){
    std::cout << "No triangles found in mesh file" << mesh_path << std::endl;
    exit(-1);
  }
  point.resize(np);
  tria.resize(nt);
  if(np)
    normal.resize(np);
  if(nNAtV)
    NormalAtVertices.resize(nNAtV + 1);

  GmfGotoKwd(inm,GmfVertices);
  for (k=0; k<np; k++){
    ppt = &point[k];
    if ( ver == GmfFloat ){
      GmfGetLin(inm,GmfVertices,&fp1,&fp2,&fp3,&ppt->ref);
      ppt->c[0] = fp1;
      ppt->c[1] = fp2;
      ppt->c[2] = fp3;
    }
    else
      GmfGetLin(inm,GmfVertices,&ppt->c[0],&ppt->c[1],&ppt->c[2],&ppt->ref);
  }
  GmfGotoKwd(inm,GmfTriangles);
  for (k=0; k<nt; k++) {
    pt = &tria[k];
    GmfGetLin(inm,GmfTriangles,&pt->v[0],&pt->v[1],&pt->v[2],&pt->ref);
  }
  if ( nn ) {
    GmfGotoKwd(inm,GmfNormals);
    for (k=0; k<nn; k++) {
      if ( ver == GmfFloat ) {
	GmfGetLin(inm,GmfNormals,&fp1,&fp2,&fp3);
	normal[k].n[0] = fp1;
	normal[k].n[1] = fp2;
	normal[k].n[2] = fp3;
      }
      else
	GmfGetLin(inm,GmfNormals,&normal[k].n[0],&normal[k].n[1],&normal[k].n[2]);
      n  = normal[k].n;
      dd = 1.0 / sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
      n[0] *= dd;
      n[1] *= dd;
      n[2] *= dd;
    }
  }

  //VERTICES
  int inv = -1;//((pcv->profile.invertVertical)?-1:1);
  for (int i = 0 ; i < point.size() ; i++){
    vertices.push_back(      point[i].c[0]);
    vertices.push_back(inv * point[i].c[1]);
    vertices.push_back(inv * point[i].c[2]);
  }
  //INDICES
  for (int i = 0 ; i < tria.size() ; i++){
    for(int j = 0 ; j < 3 ; j++)
      indices.push_back(tria[i].v[j]-1);}
  //NORMALS
  if ( nNAtV ) {
    GmfGotoKwd(inm,GmfNormalAtVertices);
    for (k=0; k<nNAtV; k++)
      GmfGetLin(inm,GmfNormalAtVertices,
		&NormalAtVertices[k].inds[0],
		&NormalAtVertices[k].inds[1]);
  }
  if(nn && nNAtV){
    for(int i = 0 ; i < vertices.size() ; i++)
      normals.push_back(0.0f);
    for(int i = 0 ; i < NormalAtVertices.size() - 1 ; i++){
      int indV = NormalAtVertices[i].inds[0] - 1;
      int indN = NormalAtVertices[i].inds[1] - 1;
      normals[3 * indV + 0] =       normal[indN].n[0];
      normals[3 * indV + 1] = inv * normal[indN].n[1];
      normals[3 * indV + 2] = inv * normal[indN].n[2];
    }
  }

  std::cout << "Succesfully opened  " << mesh_path << std::endl;
  GmfCloseMesh(inm);


  //Lecture du .sol
  std::string solFile = meshFile.substr(0, meshFile.size()-5) + ".sol";
  int ver2, dim2;
  int inMeshSol = 0;
  inMeshSol = GmfOpenMesh((char*)(solFile.c_str()), GmfRead, &ver2, &dim2);

  if(inMeshSol){
    int type, offset, typtab[GmfMaxTyp];
    int nSol      = GmfStatKwd(inMeshSol, GmfSolAtVertices, &type, &offset, &typtab);
    std::vector<float> values(nSol);
    GmfGotoKwd(inMeshSol, GmfSolAtVertices);
    for(int i = 0 ; i< nSol ; i++){
      double val;
      if ( ver2 == GmfFloat ){
	GmfGetLin(inMeshSol, GmfSolAtVertices, &values[i]);
      }
      else{
	GmfGetLin(inMeshSol, GmfSolAtVertices, &val);
	values[i] = val;
      }
    }
    std::cout << "Succesfully opened  " << solFile << std::endl;
    GmfCloseMesh(inMeshSol);

    //From palette
    float mini = 1e9;
    float maxi = -1e9;
    for(int i = 0 ; i < values.size() ; i++){
      mini = std::min(mini, values[i]);
      maxi = std::max(maxi, values[i]);
    }

    std::vector<glm::vec4> P;
    P.push_back(glm::vec4(0.0f,   1.0f,   0.0f,   0.0f));
    P.push_back(glm::vec4(0.35, 0.75,   0.75, 0.0f));
    P.push_back(glm::vec4(0.5, 0.0f, 1.0f, 0.0f));
    P.push_back(glm::vec4(0.65, 0.0f, 0.75, 0.75));
    P.push_back(glm::vec4(1.0f,   0.0f, 0.0f, 1.0f));

    for(int k = 0 ; k < values.size() ; k++){
      float val = (maxi - values[k]) / (maxi - mini);
      for(int i = 0 ; i < P.size() ; i++){
	if(i==P.size()-1){
	  float fac       = (val - P[i-1][0])/(P[i][0] - P[i-1][0]);
	  glm::vec4 col   = (1-fac)*P[i-1] + fac*P[i];
	  colors.push_back(col[1]);
	  colors.push_back(col[2]);
	  colors.push_back(col[3]);
	}
	else{
	  if( (val>=P[i][0]) && (val<=P[i+1][0]) ){
	    float fac       = (val - P[i][0])/(P[i+1][0] - P[i][0]);
	    glm::vec4 col   = (1-fac)*P[i] + fac*P[i+1];
	    colors.push_back(col[1]);
	    colors.push_back(col[2]);
	    colors.push_back(col[3]);
	    break;
	  }
	}
      }
    }
  }
  else{
    std::cout << "Failed to open .sol " << solFile << std::endl;
  }
}

void Object::createAndBindBuffers(){
  createVAO(&VAO);
  createBuffer(&mBuffer, &vertices);
  createBuffer(&nBuffer, &normals);
  createBuffer(&cBuffer, &colors);
  createBuffer(&iBuffer, &indices);

  int shaderID = parentScene->parentWindow->parentContext->shader->ID;
  glUseProgram(shaderID);
  glBindVertexArray(VAO);
  bindBuffer(0, shaderID, mBuffer, "vertex_position");
  bindBuffer(1, shaderID, nBuffer, "vertex_normal");
  bindBuffer(2, shaderID, cBuffer, "vertex_color");
  bindIndicesBuffer(iBuffer);

  context->window->controls->lighting = 1;
  if(nBuffer!=-1)
    context->window->controls->lighting = 2;
  if(cBuffer!=-1)
    context->window->controls->colors = 1;
    
}
	
void Object::render(){
  int structure = parentScene->parentWindow->controls->structure;
  GLenum cull   = parentScene->parentWindow->controls->cull;
  int    ID     = parentScene->parentWindow->parentContext->shader->ID;

  GLenum render;
  switch(structure){
  case(0): render = GL_FILL;  break;
  case(1): render = GL_FILL;  break;
  case(2): render = GL_LINE;  break;
  case(3): render = GL_POINT; break;
  }

  //OpenGL statements
  glUseProgram(ID);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0,1.0);
  glPolygonMode(GL_FRONT_AND_BACK, render);

  //Culling and face/edge/point
  if(cull == GL_BACK){
    if(render == GL_FILL){
      glEnable(GL_CULL_FACE);
      glCullFace(cull);
    }
    else
      glDisable(GL_CULL_FACE);
  }
  else if(cull == GL_FRONT){
    glEnable(GL_CULL_FACE);
    glCullFace(cull);
  }

  //Transformations
  if(context->window->controls->animate)
    MODEL = glm::rotate(glm::mat4(1), 0.01f, parentScene->view->up) * MODEL;
  glm::mat4 MVP = (parentScene->view->PROJ) * (parentScene->view->VIEW) * MODEL;

  //According parameters to the mesh
  if(cBuffer == -1){
    if(context->window->controls->colors == 1)
      context->window->controls->colors = 2;
  }
  if(nBuffer == -1){
    if(context->window->controls->lighting == 2)
      context->window->controls->lighting = 0;
    if(context->window->controls->colors == 2)
      context->window->controls->colors = 0;
  }
 
  //Uniform sending
  send(ID, MVP, 	                        "MVP");
  send(ID, MODEL, 	                        "M");
  send(ID, parentScene->view->VIEW,             "V");
  send(ID, context->window->controls->lighting, "uLighting");
  send(ID, context->window->controls->colors,   "uColor");
  send(ID, context->window->controls->structure,"uStructure");
  send(ID, glm::vec3(1,1,1),                    "objectColor");
  send(ID, 0,                                   "uSecondPass");

  //Drawing
  glBindVertexArray(VAO);
  if(context->window->controls->structure==1 || ((context->window->controls->structure == 0) && (context->window->controls->colors == 0) && (context->window->controls->lighting == 0))){
    send(ID, glm::vec3(0,0,0), "objectColor");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glDisable(GL_POLYGON_OFFSET_FILL);
    send(ID, 1,                "uSecondPass");
    send(ID, glm::vec3(1,1,1), "objectColor");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
  }
  else
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);  
}	
