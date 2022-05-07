/* Generated by Edge Impulse
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_
#define _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_

#include <ei/anomaly/anomaly.h>

// (before - mean) / scale
const float ei_classifier_anom_scale[EI_CLASSIFIER_ANOM_AXIS_SIZE] = { 3.1710069914577828, 1.0213466481529405, 3.1382596100438516 };
const float ei_classifier_anom_mean[EI_CLASSIFIER_ANOM_AXIS_SIZE] = { 3.5804998670270343, 0.8296244903946871, 3.766300126524569 };

const ei_classifier_anom_cluster_t ei_classifier_anom_clusters[EI_CLASSIFIER_ANOM_CLUSTER_COUNT] = { { { -0.9229515194892883, -0.4391716718673706, 0.3682485818862915 }, 0.3576760673321244 }
, { { -0.002447117120027542, -0.3636125326156616, -1.1065174341201782 }, 0.37316379385801574 }
, { { 0.9766618609428406, -0.22014546394348145, 0.6069228649139404 }, 0.2845644652908755 }
, { { 0.8908360600471497, 0.03893580660223961, 0.8465798497200012 }, 0.3456964444307295 }
, { { -1.0482581853866577, 3.187594175338745, -1.1791273355484009 }, 0.46403330602598636 }
, { { -1.1284681558609009, -0.8099610209465027, -1.1988089084625244 }, 0.7807168775753492 }
, { { 1.3430912494659424, 0.005060381721705198, 0.8258402347564697 }, 0.3734413643679454 }
, { { -0.9654417037963867, 0.5436707139015198, 1.535154938697815 }, 0.526665160212511 }
, { { -1.066036343574524, 5.010214328765869, -1.1797022819519043 }, 0.3125165732989562 }
, { { 1.2874226570129395, -0.2744923233985901, 0.7813891172409058 }, 0.3317913323829799 }
, { { 1.1635953187942505, 0.6238871216773987, 0.887161910533905 }, 0.36098976365251284 }
, { { 1.144539475440979, -0.0756371021270752, 1.1158322095870972 }, 0.4678912261304319 }
, { { 0.8860992789268494, -0.26119181513786316, 0.840965747833252 }, 0.3196047536463165 }
, { { 1.3899641036987305, 0.873373806476593, 0.9192684888839722 }, 0.3694867721129626 }
, { { 1.027860403060913, 0.3477703034877777, 0.6458823680877686 }, 0.37623502557003297 }
, { { 0.32207542657852173, -0.11094292998313904, -1.116873860359192 }, 0.3478628949319081 }
, { { 0.31615665555000305, -0.3870435953140259, -1.147192358970642 }, 0.29454821091171685 }
, { { -0.9564409255981445, -0.3839397728443146, 0.6590589880943298 }, 0.3559353441565317 }
, { { 0.7379062175750732, 0.0679217278957367, -1.161972999572754 }, 0.29406223421148836 }
, { { -1.0487293004989624, 3.654374837875366, -1.1764429807662964 }, 0.38667229507775946 }
, { { 0.32256025075912476, 0.24826891720294952, -1.158054232597351 }, 0.42516395488052205 }
, { { -0.9678630828857422, 0.6035979390144348, 1.102872371673584 }, 0.5837538872321222 }
, { { 0.7843086123466492, -0.2031414955854416, 1.5501632690429688 }, 0.40578382788483486 }
, { { -0.9954106211662292, 0.1339821219444275, 0.04728129878640175 }, 0.4421156040119976 }
, { { 0.8992307782173157, -0.23446336388587952, 1.2342240810394287 }, 0.37144533210023467 }
, { { 1.0398856401443481, -0.01795443706214428, 0.4391615092754364 }, 0.3861104957324132 }
, { { -1.0263750553131104, 0.20189376175403595, 0.4498642385005951 }, 0.5491357992715807 }
, { { 1.341904878616333, -0.163704052567482, 0.31761589646339417 }, 0.4181657899339741 }
, { { -0.8826528787612915, 1.2273880243301392, 1.8926169872283936 }, 0.3001591557354028 }
, { { -0.5020335912704468, -0.34191447496414185, 1.4415043592453003 }, 0.4957001402731907 }
, { { 1.2257753610610962, -0.1723652184009552, -1.1519078016281128 }, 0.21690161875595518 }
, { { -1.047031044960022, 4.430507183074951, -1.1768088340759277 }, 0.2874313913292491 }
};

#endif // _EI_CLASSIFIER_ANOMALY_CLUSTERS_H_
