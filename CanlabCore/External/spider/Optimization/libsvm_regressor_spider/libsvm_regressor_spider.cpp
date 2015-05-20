/* $Revision: 1.3 $ */
// Automatically generated by MATLAB Project Wizard version 1.0

//
// This is the gateway routine for a MATLAB Math/Graphics Library-based
// C MATLAB MEX File.

#include <memory.h>


#include <mex.h>

char Versionstring[]="2.81";



double *Custom_Kernel;
int Custom_M,Custom_N;



#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

#include "svm.h"

//
struct svm_model
{
	svm_parameter param;	// parameter
	int nr_class;		// number of classes, = 2 in regression/one class svm
	int l;			// total #SV
	svm_node **SV;		// SVs (SV[l])
	double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[n-1][l])
	double *rho;		// constants in decision functions (rho[n*(n-1)/2])
	
	// for classification only
	
	int *label;		// label of each class (label[n])
	int *nSV;		// number of SVs for each class (nSV[n])
				// nSV[0] + nSV[1] + ... + nSV[n-1] = l
	// XXX
	int free_sv;		// 1 if svm_model is created by svm_load_model
				// 0 if svm_model is created by svm_train
};




extern "C"{
	
/*
-s svm_type : set type of SVM (default 0)
0 -- C-SVC
1 -- nu-SVC
2 -- one-class SVM
3 -- epsilon-SVR
4 -- nu-SVR
-t kernel_type : set type of kernel function (default 2)
0 -- linear: u'*v
1 -- polynomial: (gamma*u'*v + coef0)^degree
2 -- radial basis function: exp(-gamma*|u-v|^2)
3 -- sigmoid: tanh(gamma*u'*v + coef0)
-d degree : set degree in kernel function (default 3)
-g gamma : set gamma in kernel function (default 1/k)
-r coef0 : set coef0 in kernel function (default 0)
-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
-m cachesize : set cache memory size in MB (default 40)
-e epsilon : set tolerance of termination criterion (default 0.001)
-h shrinking: whether to use the shrinking heuristics, 0 or 1 (default 1)
-wi weight: set the parameter C of class i to weight*C, for C-SVC (default 1)
-v n: n-fold cross validation mode
	*/
	
	void mexFunction(
		int nlhs,              // Number of left hand side (output) arguments
		mxArray *plhs[],       // Array of left hand side arguments
		int nrhs,              // Number of right hand side (input) arguments
		const mxArray *prhs[]  // Array of right hand side arguments
		)
		
		/*
		prhs =  [X,Y , s , t , d , g ,r , c , n ,p , m ,e ,h ,wi,v];
		*/
	{
		
		//mexPrintf("start mexfunction %d ",nrhs)	;
		//if(nrhs==0)
		//	return;
		
		int elements,max_index,i,j;
		
		struct svm_parameter param;		// set by parse_command_line
		struct svm_problem prob;		// set by read_problem
		struct svm_model *model;
		struct svm_node *x_space;

		
		if(nrhs<15)
		{		
			mexPrintf("Libsvm interface. Used Libsvm version : %s\n",Versionstring);
			mexPrintf("Usage: [alpha,xSV,bias0]=libsvm_classifier_spider(x,y,svm_type,kernelType,degree,gamma,coef0,nu,cachesize,C,eps,p,weight_label,weight,nr_weight,K);\n");
			return;
		}
		else
		if(nrhs==16)
		{
			
			
			Custom_M = mxGetM( prhs[15]);
			Custom_N = mxGetN( prhs[15]);
			
			mexPrintf("Using Custom Kernel %d %d\n",Custom_M,Custom_N);
			
			Custom_Kernel = Malloc( double, Custom_M*Custom_N);

			if ( Custom_Kernel)
				memcpy( Custom_Kernel, mxGetData( prhs[15]), Custom_M*Custom_N*sizeof( double));
			else
			{
				mexErrMsgTxt("No Memory for Kernel Matrix");
				return;
			}
			
		}
        

		
		double * x=mxGetPr(prhs[0]);
		double * y=mxGetPr(prhs[1]);
		
		double svm_type=mxGetScalar(prhs[2]);
		double kernelType=mxGetScalar(prhs[3]);
		double degree=mxGetScalar(prhs[4]);
		double gamma=mxGetScalar(prhs[5]);
		double coef0=mxGetScalar(prhs[6]);
		
		double nu=mxGetScalar(prhs[7]);
		
		double cachesize=mxGetScalar(prhs[8]);
		double C=mxGetScalar(prhs[9]);
		double eps=mxGetScalar(prhs[10]);
		double p=mxGetScalar(prhs[11]);
		double shrinking=mxGetScalar(prhs[12]);
		
		//double * weight=mxGetPr(prhs[13]);
		double nr_weight=mxGetScalar(prhs[13]);
		
	//	mexPrintf("got params %d",nr_weight)	;
		
		
		
		memset(&param,0,sizeof(param));
		param.cache_size=cachesize;
		param.C=C;
		param.eps=eps;
		param.p=p;
		param.shrinking=(int)shrinking;
		param.nr_weight=(int)nr_weight;
		param.nu=nu;
		param.coef0=coef0;
		param.gamma=gamma;
		param.degree=degree;
		param.kernel_type=kernelType;
		param.svm_type=(int)svm_type;
		
		// prepare libsvm data structure
		prob.l=mxGetM(prhs[0]);
		elements = mxGetN(prhs[0]);
		
//		mexPrintf("%d %d %f\n",prob.l,elements,nu);
		
		// alloc memory for data structure
		prob.y = Malloc(double,prob.l);
		prob.x = Malloc(struct svm_node *,prob.l);
		x_space = Malloc(struct svm_node,prob.l*(elements+1));
		
		max_index = 0;
		j=0;
		
		// fill libsvm structure in memory
//		mexPrintf("fill libsvm structure in memory")	;
		
		max_index = 0;
		j=0;
		
		for(i=0;i<prob.l;i++)
		{
			double label;
			prob.x[i] = &x_space[i*(elements+1)];
			
			
			label = y[i];
			prob.y[i] = label;
			
			for ( j=0; j < elements;j++)
			{
				
				x_space[i*(elements+1)+j].index=j+1;	
				x_space[i*(elements+1)+j].value=x[j*prob.l +i];
				
			}	
			
			x_space[i*(elements+1)+elements].index=-1;
		}	
		mexPrintf("\n");
		
		
                //~ for(i=0;i<prob.l;i++)
                //~ {
                    //~ mexPrintf("%f\t",prob.y[i]);
                    //~ for ( j=0; j < elements+1;j++)
                    //~ {
                     //~ mexPrintf("%d, %f\t",prob.x[i][j].index,prob.x[i][j].value);
                    //~ }
                    //~ mexPrintf("\n");
                //~ }
		
		
		mexPrintf("training...");
		
		model = svm_train(&prob,&param);
		mexPrintf("finished training\n");
		
		
		// alpha
		
		if(nlhs>0)	
		{	
                        plhs[0]=mxCreateDoubleMatrix(model->l,1,mxREAL);
                        double *p=mxGetPr(plhs[0]);
                        
                        for (int i=0;i<model->l;i++)
                            p[i]= model->sv_coef[0][i];
                        
                        //~ mexPrintf("%d alpha copied\n",model->l);
			
		}	
		
		if(nlhs>1)	
		{
			plhs[1]=mxCreateDoubleMatrix(model->l,elements,mxREAL);		
			double *xsv=mxGetPr(plhs[1]);
                    
                    
                    //~ mexPrintf("%d Elements per row\n",elements);
                    //~ mexPrintf("%d %d\n",model->l,elements);
			
                    for(i=0;i<model->l;i++)
			{
				
				for(j=0;j<elements;j++)
				{
                                    //~ mexPrintf("%d %d %d:%f |", i,j,model->SV[i][j].index,model->SV[i][j].value);
					xsv[j*model->l+i] = model->SV[i][j].value;
                                }
                                //mexPrintf("\n");
			}
                        
                        //mexPrintf("XSV copied\n");
			
			
		}
		
		if(nlhs>2)	
		{
			plhs[2]=mxCreateDoubleMatrix(1,1,mxREAL);
                        double *b=mxGetPr(plhs[2]);
			 *b=-model->rho[0];
                        //mexPrintf("b0 copied\n");
		}
		
		if(nlhs>3)	
		{
				plhs[3]=mxCreateDoubleMatrix(1,1,mxREAL);
			*mxGetPr(plhs[3]) =param.C;
		}
		

                 //mexPrintf("%returning\n");

		svm_destroy_model(model);
		
		// free memory 
		free(prob.y);
		free(prob.x);	
		free(x_space);
		
	}
	
}