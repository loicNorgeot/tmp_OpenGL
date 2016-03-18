#include "object.h"

extern "C" {
#include <libmesh5.h>
}

#define FLOAT_MAX 1.e20
float alpha = 0.0f;

Object::Object(){
  createGeometry();
}
Object::Object(Object* obj){
  vertices = obj->vertices;
  colors = obj->colors;
  normals = obj->normals;
  indTri = obj->indTri;
  meshfile = obj->meshfile;
  name  = split(meshfile, '/').back();
  std::cout << name << std::endl;
  MODEL = glm::rotate(glm::mat4(1), 3.14159f, glm::vec3(1,0,0));
}
Object::Object(char * mesh_path){
  meshfile = mesh_path;
  name  = split(meshfile, '/').back();
  std::cout << name << std::endl;
  createGeometry(mesh_path);
  MODEL = glm::rotate(glm::mat4(1), 3.14159f, glm::vec3(1,0,0));
}
void Object::computeID(int _id){
  id = _id;
  pickingColor = glm::vec3( (id%255)/255. + 1/255. , (id/255)/255. , 0 );
}
void Object::init(Scene* scene){
  selected = false;
  parentScene = scene;
  computeBoundingBox();
  scaleAndTranslate();
  createAndBindBuffers();
  computeID(context->window->scene->objects.size()+1);

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
  size    = bbmax - bbmin;
  float maxDim      = std::max( std::max(size.x, size.y) , size.z );

  //Scle and translation for initial positionning
  translation = -0.5f * (bbmin + bbmax);

  //Used for trespassing
  size = 0.5f * (bbmax-bbmin);;

  parentScene->scale = 1.0f;// * std::max( 0.5f/maxDim, parentScene->scale);
  //scale       = parentScene->scale;// / maxDim;
}
void Object::scaleAndTranslate(){
  for(int i = 0 ; i < vertices.size() ; i+=3){
    for(int j = 0 ; j < 3 ; j++){
      vertices[i+j] += translation[j];
      //vertices[i+j] *= scale;
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
  indTri = std::vector<int>{
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
  int nPts, nTri, nNor, nTet, nNorAtV;
  int ver, dim;
  double* tmp = new double[3];

  //READING .mesh
  int inm = GmfOpenMesh(mesh_path,GmfRead,&ver,&dim);
  if ( !inm ){
    std::cout << "Unable to open mesh file " << mesh_path << std::endl;
    exit(-1);
  }

  //GETTING SIZES
  nPts    = GmfStatKwd(inm, GmfVertices);
  nTri    = GmfStatKwd(inm, GmfTriangles);
  nTet    = GmfStatKwd(inm, GmfTetrahedra);
  nNor    = GmfStatKwd(inm, GmfNormals);
  nNorAtV = GmfStatKwd(inm, GmfNormalAtVertices);
  if ( !nPts || !nTri ){
    std::cout << "Missing data in mesh file" << mesh_path << std::endl;
    exit(-1);
  }
  vertices.resize(3 * nPts);
  refVert.resize(nPts);
  indTri.resize(3 * nTri);
  refTri.resize(nTri);
  indTet.resize(4 * nTet);
  refTet.resize(nTet);


  //VERTICES & INDICES
  GmfGotoKwd(inm,GmfVertices);
  for (int k = 0; k < nPts; k++){
    GmfGetLin(inm,GmfVertices,&tmp[0],&tmp[1],&tmp[2], &refVert[k]);
    vertices[3*k + 0] = tmp[0];
    vertices[3*k + 1] = tmp[1];
    vertices[3*k + 2] = tmp[2];
  }
  GmfGotoKwd(inm,GmfTriangles);
  for (int k = 0; k < nTri; k++){
    GmfGetLin(inm,GmfTriangles,&indTri[3*k],&indTri[3*k+1], &indTri[3*k+2], &refTri[k]);
    indTri[3*k]-=1;
    indTri[3*k+1]-=1;
    indTri[3*k+2]-=1;
  }
  nbVertices = vertices.size()/3;

  //TETRAHEDRON
  if(nTet){
    GmfGotoKwd(inm,GmfTetrahedra);
    for (int k = 0 ; k < nTet ; k++)
      GmfGetLin(inm,GmfTetrahedra,&indTet[4*k], &indTet[4*k+1], &indTet[4*k+2], &indTet[4*k+3],&refTet[k]);
  }

  //NORMALS
  std::vector<float> tmp_normals;
  std::vector<int> NormalAtVertices;
  if(nNor)
    tmp_normals.resize(3 * nNor);
    normals.resize(3 * nNor);
  if(nNorAtV)
    NormalAtVertices.resize(2 * nNorAtV + 2);

  if(nNor && nNorAtV){
    GmfGotoKwd(inm,GmfNormals);
    for (int k = 0 ; k < nNor ; k++) {
      GmfGetLin(inm,GmfNormals,&tmp[0],&tmp[1],&tmp[2]);
      double dd = 1.0 / std::sqrt(tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2]);
      tmp_normals[3*k + 0] = tmp[0] * dd;
      tmp_normals[3*k + 1] = tmp[1] * dd;
      tmp_normals[3*k + 2] = tmp[2] * dd;
    }
    GmfGotoKwd(inm,GmfNormalAtVertices);
    for (int k = 0 ; k < nNorAtV; k++)
      GmfGetLin(inm,GmfNormalAtVertices, &NormalAtVertices[2*k+0], &NormalAtVertices[2*k+1]);
    for(int i = 0 ; i < nNorAtV - 1 ; i++){
      int indV = NormalAtVertices[2*i+0] - 1;
      int indN = NormalAtVertices[2*i+1] - 1;
      normals[3 * indV + 0] =  tmp_normals[3*indN+0];
      normals[3 * indV + 1] =  tmp_normals[3*indN+1];
      normals[3 * indV + 2] =  tmp_normals[3*indN+2];
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
      if ( ver2 == GmfFloat )
	GmfGetLin(inMeshSol, GmfSolAtVertices, &values[i]);
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
  createBuffer(&iBuffer, &indTri);

  int shaderID = parentScene->parentWindow->shader->ID;
  glUseProgram(shaderID);
  glBindVertexArray(VAO);
  bindBuffer(0, shaderID, mBuffer, "vertex_position");
  bindBuffer(1, shaderID, nBuffer, "vertex_normal");
  bindBuffer(2, shaderID, cBuffer, "vertex_color");
  bindIndicesBuffer(iBuffer);
}

void Object::render(){


  /*
  if(cBuffer!=-1){
    for(int i = 0 ; i < colors.size() ; i+=3){
      glm::vec3 col(colors[i], colors[i+1], colors[i+2]);
      col = glm::rotate(col, 0.01f, glm::vec3(1,1,1));
      for(int j = 0 ; j < 3 ; j++)
	colors[i+j] = col[j];
    }
    updateBuffer(cBuffer, &colors);
  }

  if(nBuffer != -1){
    alpha += 0.025f;
    for(int i = 0 ; i < vertices.size() ; i+=3){
      glm::vec3 pos(vertices[i], vertices[i+1], vertices[i+2]);
      glm::vec3 nor(normals[i], normals[i+1], normals[i+2]);
      pos += 0.00025f*std::sin(alpha) * nor;
      for(int j = 0 ; j < 3 ; j++)
        vertices[i+j] = pos[j];
    }
    updateBuffer(mBuffer, &vertices);
  }
  */

  int structure = parentScene->parentWindow->controls->structure;
  GLenum cull   = parentScene->parentWindow->controls->cull;
  int    ID     = parentScene->parentWindow->shader->ID;

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
  //if(clipped)
  glDisable(GL_CULL_FACE);

  //Transformations
  glm::mat4 MVP = parentScene->view->PROJ * parentScene->view->VIEW * glm::scale(MODEL, glm::vec3(parentScene->scale));

  //According parameters to the mesh
  if(cBuffer == -1){
    if(parentScene->parentWindow->controls->colors == 1)
      parentScene->parentWindow->controls->colors = 2;
  }
  if(nBuffer == -1){
    if(parentScene->parentWindow->controls->lighting == 2)
      parentScene->parentWindow->controls->lighting = 0;
    if(parentScene->parentWindow->controls->colors == 2)
      parentScene->parentWindow->controls->colors = 0;
  }

  glm::mat4 M = glm::scale(MODEL, glm::vec3(parentScene->scale));
  //Uniform sending
  send(ID, MVP, 	                              "MVP");
  send(ID, M,                                   "M");
  send(ID, parentScene->view->VIEW,             "V");
  send(ID, parentScene->parentWindow->controls->lighting, "uLighting");
  send(ID, parentScene->parentWindow->controls->colors,   "uColor");
  send(ID, parentScene->parentWindow->controls->structure,"uStructure");
  if(selected)
    send(ID, glm::vec3(1,0.8,0),                  "objectColor");
  else
    send(ID, glm::vec3(1,1,1),                  "objectColor");
  send(ID, 0,                                   "uSecondPass");
  send(ID, 0, "picking");
  send(ID, (int)clipped,"clipping");

  //Drawing
  glBindVertexArray(VAO);
  if(parentScene->parentWindow->controls->structure==1 || ((parentScene->parentWindow->controls->structure == 0) && (parentScene->parentWindow->controls->colors == 0) && (parentScene->parentWindow->controls->lighting == 0))){
    send(ID, glm::vec3(0,0,0), "objectColor");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, indTri.size(), GL_UNSIGNED_INT, (void*)0);

    glDisable(GL_POLYGON_OFFSET_FILL);
    send(ID, 1,                "uSecondPass");
    send(ID, glm::vec3(1,1,1), "objectColor");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, indTri.size(), GL_UNSIGNED_INT, (void*)0);
  }
  else
    glDrawElements(GL_TRIANGLES, indTri.size(), GL_UNSIGNED_INT, (void*)0);
  send(ID, 0,"clipping");
  glBindVertexArray(0);
}
void Object::pickingRender(){
  int    ID     = parentScene->parentWindow->shader->ID;
  glBindVertexArray(VAO);
  glUseProgram(ID);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glm::mat4 MVP = (parentScene->view->PROJ) * (parentScene->view->VIEW) * glm::scale(MODEL, glm::vec3(parentScene->scale));
  send(ID, MVP,"MVP");
  send(ID, 0, "uColor");
  send(ID, 1, "picking");
  send(ID, pickingColor, "objectColor");
  glDrawElements(GL_TRIANGLES, indTri.size(), GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
  if(parentScene->ground)
    parentScene->background->render();
  glFlush();
}



/*
//data structures
typedef struct {
  int    min,ind,nxt,elt;
} Cell;

typedef struct {
  Cell  *cell;
  int    nmax,hsiz,hnxt;
} Htab;

// insert edge a,b and update adjacent triangle
static int hcode_2d(Tria *tria,Htab *ht,int a,int b,int k,int i) {
  Cell     *pc;
  pTria     pt,pt1;
  int       abmin,adj,sum;

  sum = a+b;
  if ( sum >= ht->nmax )  return(0);

  // check if edge ab stored
  pc    = &ht->cell[sum];
  abmin = NS_MIN(a,b);
  if ( !pc->min ) {
    pc->min = abmin;
    pc->elt = k;
    pc->ind = i;
    return(1);
  }

  // analyze linked list
  pt  = &tria[k];
  do {
    pt1 = &tria[pc->elt];
    if ( pc->min == abmin ) {
      adj = pt1->adj[pc->ind];
      if ( !adj ) {
        pt->adj[i]        = 3*pc->elt+pc->ind;
        pt1->adj[pc->ind] = 3*k+i;
      }
      return(1);
    }
    else if ( !pc->nxt ) {
      pc->nxt = ht->hnxt;
      pc      = &ht->cell[ht->hnxt];
      if ( !pc )  return(0);
      pc->min  = abmin;
      pc->elt  = k;
      pc->ind  = i;
      ht->hnxt = pc->nxt;
      pc->nxt  = 0;

      // check for size overflow
      if ( !ht->hnxt )  return(0);
      return(1);
    }
    pc = &ht->cell[pc->nxt];
  } while (1);

  return(0);
}


// build adjacency table
int hashel_2d(NSst *nsst) {
  Htab     ht;
  pTria    pt;
  pPoint   ppt;
  int      k,na;
  char     i,i1,i2;

  if ( nsst->info.verb == '+' )  fprintf(stdout,"    Adjacency table: ");

  // alloc hash
  ht.nmax = (int)(3.71 * nsst->info.np);
  ht.cell = (Cell*)calloc(ht.nmax+2,sizeof(Cell));
  assert(ht.cell);

  ht.hsiz = 2 * nsst->info.np;
  ht.hnxt = ht.hsiz;
  for (k=ht.hsiz; k<ht.nmax; k++)
    ht.cell[k].nxt = k+1;

  // update adjacency
  na = 0;
  for (k=1; k<=nsst->info.nt; k++) {
    pt = &nsst->mesh.tria[k];
    for (i=0; i<3; i++) {
      i1 = (i+1) % 3;
      i2 = (i+2) % 3;
      if ( !hcode_2d(nsst->mesh.tria,&ht,pt->v[i1],pt->v[i2],k,i) )  return(0);
      na++;
    }
  }

  // add seed with point
  for (k=1; k<=nsst->info.nt; k++) {
    pt = &nsst->mesh.tria[k];
    for (i=0; i<3; i++) {
      if ( !pt->adj[i] )  nsst->mesh.point[pt->v[1]].s = k;
    }
  }
  for (k=1; k<=nsst->info.nt; k++) {
    pt = &nsst->mesh.tria[k];
    for (i=0; i<3; i++) {
      ppt = &nsst->mesh.point[pt->v[i]];
      if ( !ppt->s )  ppt->s = k;
    }
  }
  free(ht.cell);

  if ( nsst->info.verb == '+' )  fprintf(stdout," %d updated\n",na);

  return(1);
}
*/
